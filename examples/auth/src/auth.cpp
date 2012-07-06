#if defined(__WIN32__) || defined(_WIN32)
#include <rpc.h>
#else
#include <unistd.h>
#include <fcntl.h>
#endif
#if defined(YB_USE_WX)
#include <wx/app.h>
#elif defined(YB_USE_QT)
#include <QCoreApplication>
#endif
#include <iostream>
#include <util/str_utils.hpp>
#include <util/xmlnode.h>
#include <orm/SqlPool.h>
#include <orm/MetaDataSingleton.h>
#include "md5.h"
#include "logger.h"
#include "micro_http.h"
#include "domain/User.h"
#include "domain/LoginSession.h"

using namespace std;
using namespace Domain;
using namespace Domain;

std::auto_ptr<Yb::Engine> engine;

Yb::LongInt
get_random()
{
    Yb::LongInt buf;
#if defined(__WIN32__) || defined(_WIN32)
    UUID new_uuid;
    UuidCreate(&new_uuid);
    buf = new_uuid.Data1;
    buf <<= 32;
    Yb::LongInt buf2 = (new_uuid.Data2 << 16) | new_uuid.Data3;
    buf += buf2;
#else
    int fd = open("/dev/urandom", O_RDONLY);
    if (fd == -1)
        throw std::runtime_error("can't open /dev/urandom");
    if (read(fd, &buf, sizeof(buf)) != sizeof(buf)) {
        close(fd);
        throw std::runtime_error("can't read from /dev/urandom");
    }
    close(fd);
#endif
    return buf;
}

Yb::LongInt
get_checked_session_by_token(Yb::Session &session,
        StringMap &params, int admin_flag=0)
{
#if !defined(YB_USE_TUPLE)
    typedef LoginSession Row;
#else
    typedef boost::tuple<LoginSession, User> Row;
#endif
    Yb::DomainResultSet<Row> rs = Yb::query<Row>(session)
        .filter_by(Yb::filter_eq(Yb::String(_T("TOKEN")), WIDEN(params["token"])))
#if defined(YB_USE_TUPLE)
        .filter_by(Yb::Filter(Yb::String(_T("USER_ID = T_USER.ID"))))
#endif
        .all();
    Yb::DomainResultSet<Row>::iterator q = rs.begin(), qend=rs.end();
    if (q == qend)
        return -1;
#if !defined(YB_USE_TUPLE)
    LoginSession &ls = *q;
#else
    LoginSession &ls = q->get<0>();
#endif
    if (admin_flag) {
        if (ls.get_user().get_status() != 0)
            return -1;
    }
    if (Yb::now() >= ls.get_end_session())
        return -1;
    return ls.get_id();
}

std::string 
check(StringMap &params)
{
    Yb::Session session(Yb::theMetaData::instance(), engine.get());
    return get_checked_session_by_token(session, params) == -1? BAD_RESP: OK_RESP;
}

std::string 
md5_hash(const std::string &str)
{
    MD5_CTX mdContext;
    MD5Init(&mdContext);
    MD5Update(&mdContext, (unsigned char *)str.c_str(), str.size());
    MD5Final(&mdContext);
    std::string rez;
    char omg[3];
    for (int i = 0; i < 16; ++i) {
        sprintf(omg, "%02x", mdContext.digest[i]);
        rez.append(omg, 2);
    }
    return rez;
}

std::string 
registration(StringMap &params)
{
    Yb::Session session(Yb::theMetaData::instance(), engine.get());
    User::ResultSet all = Yb::query<User>(session).all();
    User::ResultSet::iterator p = all.begin(), pend = all.end();
    if (p != pend) {
        // when user table is empty it is allowed to create the first user
        // w/o password check, otherwise we should check permissions
        if (-1 == get_checked_session_by_token(session, params, 1))
            return BAD_RESP;
    }
    User::ResultSet rs = Yb::query<User>(session)
        .filter_by(Yb::filter_eq(_T("LOGIN"), WIDEN(params["login"]))).all();
    User::ResultSet::iterator q = rs.begin(), qend = rs.end();
    if (q != qend)
        return BAD_RESP;

    User user(session);
    user.set_login(WIDEN(params["login"]));
    user.set_name(WIDEN(params["name"]));
    user.set_pass(WIDEN(md5_hash(params["pass"])));
    int status;
    Yb::from_stdstring(params["status"], status);
    user.set_status(status);
    session.commit();
    return OK_RESP;
}

Yb::LongInt
get_checked_user_by_creds(Yb::Session &session, StringMap &params)
{
    User::ResultSet rs = Yb::query<User>(session,
        Yb::filter_eq(_T("LOGIN"), WIDEN(params["login"]))).all();
    User::ResultSet::iterator q = rs.begin(), qend = rs.end();
    if (q == qend)
        return -1;
    if (q->get_pass() != WIDEN(md5_hash(params["pass"])))
        return -1;
    return q->get_id();
}

std::string 
login(StringMap &params)
{
    Yb::Session session(Yb::theMetaData::instance(), engine.get());
    Yb::LongInt uid = get_checked_user_by_creds(session, params);
    if (-1 == uid)
        return BAD_RESP;

    User user(session, uid);
    while (user.get_login_sessions().begin() != user.get_login_sessions().end())
        user.get_login_sessions().begin()->delete_object();
    LoginSession login_session(session);
    login_session.set_user(user);
    Yb::LongInt token = get_random();
    login_session.set_token(Yb::to_string(token));
    login_session.set_end_session(Yb::dt_add_seconds(Yb::now(), 11*60*60));
    login_session.set_app_name(_T("auth"));
    Yb::ElementTree::ElementPtr root = Yb::ElementTree::new_element(
            _T("token"), login_session.get_token());
    session.commit();
    return root->serialize();
}

std::string
session_info(StringMap &params)
{ 
    Yb::Session session(Yb::theMetaData::instance(), engine.get());
    Yb::LongInt sid = get_checked_session_by_token(session, params);
    if (-1 == sid)
        return BAD_RESP;
    LoginSession ls(session, sid);
    return ls.xmlize(1)->serialize();
}

std::string
logout(StringMap &params)
{
    Yb::Session session(Yb::theMetaData::instance(), engine.get());
    Yb::LongInt sid = get_checked_session_by_token(session, params);
    if (-1 == sid)
        return BAD_RESP;
    LoginSession ls(session, sid);
    ls.delete_object();
    session.commit();
    return OK_RESP;
}

const Yb::String
cfg(const Yb::String &key) { return Yb::StrUtils::xgetenv(_T("YBORM_") + key); }

#if defined(YB_USE_WX)
class MyApp : public wxAppConsole {
public:
    virtual bool OnInit() { return true; }
    virtual int OnRun()
#else
    int main(int argc, char *argv[])
#endif
{
#if defined(YB_USE_QT)
    QCoreApplication app(argc, argv);
#endif
    try {
        const char *log_fname = "log.txt"; // TODO: read from config
        g_log.reset(new Log(log_fname));
        g_log->info("start log");
    }
    catch (const std::exception &ex) {
        std::cerr << "exception: " << ex.what() << "\n";
        std::cerr << "Can't open log file! Stop!\n";
        return 1;
    }
    try {
        Yb::init_default_meta();
        std::auto_ptr<Yb::SqlPool> pool(new Yb::SqlPool(YB_POOL_MAX_SIZE,
                YB_POOL_IDLE_TIME, YB_POOL_MONITOR_SLEEP, g_log.get()));
        Yb::SqlSource src(_T("auth_db"), _T("DEFAULT"),
                cfg(_T("DBTYPE")), cfg(_T("DB")), cfg(_T("USER")), cfg(_T("PASSWD")));
        pool->add_source(src);
        engine.reset(new Yb::Engine(Yb::Engine::MANUAL, pool, _T("auth_db")));
        Yb::Logger::Ptr ormlog = g_log->new_logger("orm");
        engine->set_echo(true);
        engine->set_logger(ormlog);
        int port = 9090; // TODO: read from config
        HttpHandlerMap handlers;
        handlers["/session_info"] = session_info;
        handlers["/registration"] = registration;
        handlers["/check"] = check;
        handlers["/login"] = login;
        handlers["/logout"] = logout;
        HttpServer server(port, handlers);
        server.serve();
    }
    catch (const std::exception &ex) {
        g_log->error(string("exception: ") + ex.what());
        engine.reset(NULL);
        return 1;
    }
    engine.reset(NULL);
    return 0;
}
#if defined(YB_USE_WX)
};
IMPLEMENT_APP(MyApp)
#endif
// vim:ts=4:sts=4:sw=4:et:
