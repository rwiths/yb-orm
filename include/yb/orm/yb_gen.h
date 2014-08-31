#ifndef YB_GEN_H
#define YB_GEN_H

/*
http://stackoverflow.com/questions/2361665/c-c-macro-how-to-generate-two-separate-sections-of-code-with-one-macro-boost?rq=1
*/

// Generic macro for declaring a column
#define YB_GEN_COL(prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    (YB_GEN_COL_, prop_name, _T(name), type, size, flags, default_value, _T(fk_table), _T(fk), _T(xml_name), _T(index_name))

#define YB_GEN_COL_END \
    (YB_GEN_COL_END_, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)

// Convenience macros for declaring most common cases
#define YB_GEN_COL_PK(prop_name, name) \
    YB_GEN_COL(prop_name, name, LONGINT, 0, Yb::Column::PK, YB_NULL, "", "", "", "")

#define YB_GEN_COL_FK(prop_name, name, fk_table, fk) \
    YB_GEN_COL(prop_name, name, LONGINT, 0, 0, YB_NULL, fk_table, fk, "", "")

#define YB_GEN_COL_FK_NULLABLE(prop_name, name, fk_table, fk) \
    YB_GEN_COL(prop_name, name, LONGINT, 0, Yb::Column::NULLABLE, YB_NULL, fk_table, fk, "", "")

#define YB_GEN_COL_DATA(prop_name, name, type) \
    YB_GEN_COL(prop_name, name, type, 0, Yb::Column::NULLABLE, YB_NULL, "", "", "", "")

#define YB_GEN_COL_STR(prop_name, name, size) \
    YB_GEN_COL(prop_name, name, STRING, size, Yb::Column::NULLABLE, YB_NULL, "", "", "", "")

// Generic macros for declaring sides of a one-to-many relation
#define YB_GEN_REL_ONE(one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by) \
    (YB_GEN_REL_ONE_, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, 9, 10)

#define YB_GEN_REL_MANY(one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by) \
    (YB_GEN_REL_MANY_, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, 9, 10)

// types
#define YB_GEN_CPP_TYPE_INTEGER int
#define YB_GEN_CPP_TYPE_LONGINT Yb::LongInt
#define YB_GEN_CPP_TYPE_DECIMAL Yb::Decimal
#define YB_GEN_CPP_TYPE_FLOAT double
#define YB_GEN_CPP_TYPE_DATETIME Yb::DateTime
#define YB_GEN_CPP_TYPE_STRING Yb::String
#define YB_GEN_CPP_TYPE(type__) YB_GEN_CPP_TYPE_##type__

// DO macros
#define YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    YB_GEN_EMPTY_OR_COMMA(n) prop_name
#define YB_GEN_COL_ADD_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    << prop_name
#define YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    YB_GEN_COLON_OR_COMMA(n) prop_name(name, Yb::Value::type, size, flags, default_value, fk_table, fk, xml_name, _T(#prop_name), index_name)
#define YB_GEN_PROP_DECL_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    Yb::Property<YB_GEN_CPP_TYPE(type), n> prop_name;
#define YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
    Yb::ManagedList<many_class> many_prop;
#define YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
    one_class::Holder one_prop;
#define YB_GEN_PROP_CONS_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
    , prop_name(this)
#define YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
    , many_prop(this, _T(#many_prop))
#define YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
    , one_prop(this, _T(#one_prop))
#define YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(n, prop_name, name, type, size, flags, default_value, fk_table, fk, xml_name, index_name) \
        set(n, Yb::Value(default_value));
#define YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
        { \
            Yb::Relation::AttrMap attr1, attr2; \
            attr1[_T("property")] = _T(#many_prop); \
            attr2[_T("property")] = _T(#one_prop); \
            Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, \
                _T(#one_class), attr1, _T(#many_class), attr2, cascade)); \
            rels.push_back(r); \
        }
#define YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(n, one_class, one_prop, many_class, many_prop, cascade, key, use_list, order_by, p9, p10) \
        { \
            Yb::Relation::AttrMap attr1, attr2; \
            attr1[_T("property")] = _T(#many_prop); \
            attr2[_T("property")] = _T(#one_prop); \
            Yb::Relation::Ptr r(new Yb::Relation(Yb::Relation::ONE2MANY, \
                _T(#one_class), attr1, _T(#many_class), attr2, cascade)); \
            rels.push_back(r); \
        }


// Root macros
#define YB_GEN_DECLARE(class_name__, table_name__, seq_name__, table_xml_name__, col_defs__) \
private: \
    static Yb::Tables tbls; \
    static Yb::Relations rels; \
    static Yb::DomainMetaDataCreator<class_name__> mdc; \
public: \
    typedef Yb::DomainObjHolder<class_name__> Holder; \
    struct Columns { \
        Yb::Column \
        YB_GEN_COL_MEMBERS col_defs__ \
        ; \
        Columns() \
            YB_GEN_COL_MEMB_CONS col_defs__ \
        {} \
        void fill_table(Yb::Table &tbl) \
        { \
            tbl \
            YB_GEN_COL_ADD col_defs__ \
            ; \
        } \
    }; \
    static Columns c; \
    static const Yb::String get_table_name() { return _T(table_name__); } \
    typedef Yb::DomainResultSet<class_name__> ResultSet; \
    typedef std::vector<class_name__> List; \
    typedef std::auto_ptr<List> ListPtr; \
    static ListPtr find(Yb::Session &session, \
            const Yb::Expression &filter, const Yb::Expression &order_by = Yb::Expression()) \
    { \
        class_name__::ListPtr lst(new class_name__::List()); \
        Yb::ObjectList rows; \
        session.load_collection(rows, Yb::Expression(_T(table_name__)), filter, order_by); \
        if (rows.size()) { \
            Yb::ObjectList::iterator it = rows.begin(), end = rows.end(); \
            for (; it != end; ++it) \
                lst->push_back(class_name__(*it)); \
        } \
        return lst; \
    } \
    class_name__(Yb::DomainObject *owner, const Yb::String &prop_name) \
        : Yb::DomainObject(*tbls[0], owner, prop_name) \
        YB_GEN_PROP_CONS col_defs__ \
    {} \
    class_name__() \
        : Yb::DomainObject(*tbls[0]) \
        YB_GEN_PROP_CONS col_defs__ \
    { \
        YB_GEN_PROP_DEFAULT col_defs__ \
    } \
    class_name__(const class_name__ &other) \
        : Yb::DomainObject(other) \
        YB_GEN_PROP_CONS col_defs__ \
    {} \
    explicit class_name__(Yb::Session &session) \
        : Yb::DomainObject(session.schema(), _T(table_name__)) \
        YB_GEN_PROP_CONS col_defs__ \
    { \
        YB_GEN_PROP_DEFAULT col_defs__ \
        save(session); \
    } \
    explicit class_name__(Yb::DataObject::Ptr d) \
        : Yb::DomainObject(d) \
        YB_GEN_PROP_CONS col_defs__ \
    {} \
    class_name__(Yb::Session &session, const Yb::Key &key) \
        : Yb::DomainObject(session, key) \
        YB_GEN_PROP_CONS col_defs__ \
    {} \
    class_name__(Yb::Session &session, Yb::LongInt id) \
        : Yb::DomainObject(session, _T(table_name__), id) \
        YB_GEN_PROP_CONS col_defs__ \
    {} \
    class_name__ &operator=(const class_name__ &other) \
    { \
        if (this != &other) { \
            *(Yb::DomainObject *)this = (const Yb::DomainObject &)other; \
        } \
        return *this; \
    } \
    static void create_tables_meta(Yb::Tables &tbls) \
    { \
        Yb::Table::Ptr t(new Yb::Table(_T(table_name__), _T(table_name__), _T(#class_name__))); \
        t->set_seq_name(_T(seq_name__)); \
        c.fill_table(*t); \
        tbls.push_back(t); \
    } \
    static void create_relations_meta(Yb::Relations &rels) \
    { \
        YB_GEN_CREATE_RELS col_defs__ \
    } \
    YB_GEN_PROP_DECL col_defs__ \
    struct Registrator \
    { \
        static void register_domain() { \
            Yb::theDomainFactory().register_creator(_T(table_name__), \
                Yb::CreatorPtr(new Yb::DomainCreator<class_name__>())); \
        } \
        Registrator() { register_domain(); } \
    }; \
    static Registrator registrator;

#define YB_GEN_DEFINE(class_name__) \
class_name__::Columns class_name__::c; \
Yb::Tables class_name__::tbls; \
Yb::Relations class_name__::rels; \
Yb::DomainMetaDataCreator<class_name__> class_name__::mdc(class_name__::tbls, class_name__::rels); \
class_name__::Registrator class_name__::registrator;


#define YB_GEN_COLON_OR_COMMA_0 :
#define YB_GEN_EMPTY_OR_COMMA_0 
#define YB_GEN_COLON_OR_COMMA_1 ,
#define YB_GEN_EMPTY_OR_COMMA_1 ,
#define YB_GEN_COLON_OR_COMMA_2 ,
#define YB_GEN_EMPTY_OR_COMMA_2 ,
#define YB_GEN_COLON_OR_COMMA_3 ,
#define YB_GEN_EMPTY_OR_COMMA_3 ,
#define YB_GEN_COLON_OR_COMMA_4 ,
#define YB_GEN_EMPTY_OR_COMMA_4 ,
#define YB_GEN_COLON_OR_COMMA_5 ,
#define YB_GEN_EMPTY_OR_COMMA_5 ,
#define YB_GEN_COLON_OR_COMMA_6 ,
#define YB_GEN_EMPTY_OR_COMMA_6 ,
#define YB_GEN_COLON_OR_COMMA_7 ,
#define YB_GEN_EMPTY_OR_COMMA_7 ,
#define YB_GEN_COLON_OR_COMMA_8 ,
#define YB_GEN_EMPTY_OR_COMMA_8 ,
#define YB_GEN_COLON_OR_COMMA_9 ,
#define YB_GEN_EMPTY_OR_COMMA_9 ,
#define YB_GEN_COLON_OR_COMMA_10 ,
#define YB_GEN_EMPTY_OR_COMMA_10 ,
#define YB_GEN_COLON_OR_COMMA_11 ,
#define YB_GEN_EMPTY_OR_COMMA_11 ,
#define YB_GEN_COLON_OR_COMMA_12 ,
#define YB_GEN_EMPTY_OR_COMMA_12 ,
#define YB_GEN_COLON_OR_COMMA_13 ,
#define YB_GEN_EMPTY_OR_COMMA_13 ,
#define YB_GEN_COLON_OR_COMMA_14 ,
#define YB_GEN_EMPTY_OR_COMMA_14 ,
#define YB_GEN_COLON_OR_COMMA_15 ,
#define YB_GEN_EMPTY_OR_COMMA_15 ,
#define YB_GEN_COLON_OR_COMMA_16 ,
#define YB_GEN_EMPTY_OR_COMMA_16 ,
#define YB_GEN_COLON_OR_COMMA_17 ,
#define YB_GEN_EMPTY_OR_COMMA_17 ,
#define YB_GEN_COLON_OR_COMMA_18 ,
#define YB_GEN_EMPTY_OR_COMMA_18 ,
#define YB_GEN_COLON_OR_COMMA_19 ,
#define YB_GEN_EMPTY_OR_COMMA_19 ,
#define YB_GEN_COLON_OR_COMMA_20 ,
#define YB_GEN_EMPTY_OR_COMMA_20 ,
#define YB_GEN_COLON_OR_COMMA_21 ,
#define YB_GEN_EMPTY_OR_COMMA_21 ,
#define YB_GEN_COLON_OR_COMMA_22 ,
#define YB_GEN_EMPTY_OR_COMMA_22 ,
#define YB_GEN_COLON_OR_COMMA_23 ,
#define YB_GEN_EMPTY_OR_COMMA_23 ,
#define YB_GEN_COLON_OR_COMMA_24 ,
#define YB_GEN_EMPTY_OR_COMMA_24 ,
#define YB_GEN_COLON_OR_COMMA_25 ,
#define YB_GEN_EMPTY_OR_COMMA_25 ,
#define YB_GEN_COLON_OR_COMMA_26 ,
#define YB_GEN_EMPTY_OR_COMMA_26 ,
#define YB_GEN_COLON_OR_COMMA_27 ,
#define YB_GEN_EMPTY_OR_COMMA_27 ,
#define YB_GEN_COLON_OR_COMMA_28 ,
#define YB_GEN_EMPTY_OR_COMMA_28 ,
#define YB_GEN_COLON_OR_COMMA_29 ,
#define YB_GEN_EMPTY_OR_COMMA_29 ,
#define YB_GEN_COLON_OR_COMMA_30 ,
#define YB_GEN_EMPTY_OR_COMMA_30 ,
#define YB_GEN_COLON_OR_COMMA_31 ,
#define YB_GEN_EMPTY_OR_COMMA_31 ,
#define YB_GEN_COLON_OR_COMMA_32 ,
#define YB_GEN_EMPTY_OR_COMMA_32 ,
#define YB_GEN_COLON_OR_COMMA_33 ,
#define YB_GEN_EMPTY_OR_COMMA_33 ,
#define YB_GEN_COLON_OR_COMMA_34 ,
#define YB_GEN_EMPTY_OR_COMMA_34 ,
#define YB_GEN_COLON_OR_COMMA_35 ,
#define YB_GEN_EMPTY_OR_COMMA_35 ,
#define YB_GEN_COLON_OR_COMMA_36 ,
#define YB_GEN_EMPTY_OR_COMMA_36 ,
#define YB_GEN_COLON_OR_COMMA_37 ,
#define YB_GEN_EMPTY_OR_COMMA_37 ,
#define YB_GEN_COLON_OR_COMMA_38 ,
#define YB_GEN_EMPTY_OR_COMMA_38 ,
#define YB_GEN_COLON_OR_COMMA_39 ,
#define YB_GEN_EMPTY_OR_COMMA_39 ,
#define YB_GEN_COLON_OR_COMMA_40 ,
#define YB_GEN_EMPTY_OR_COMMA_40 ,
#define YB_GEN_COLON_OR_COMMA_41 ,
#define YB_GEN_EMPTY_OR_COMMA_41 ,
#define YB_GEN_COLON_OR_COMMA(i) YB_GEN_COLON_OR_COMMA_##i
#define YB_GEN_EMPTY_OR_COMMA(i) YB_GEN_EMPTY_OR_COMMA_##i
#define YB_GEN_COL_MEMBERS_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_42
#define YB_GEN_COL_MEMBERS_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_42
#define YB_GEN_COL_MEMBERS_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_42
#define YB_GEN_COL_MEMBERS_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_41
#define YB_GEN_COL_MEMBERS_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_41
#define YB_GEN_COL_MEMBERS_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_41
#define YB_GEN_COL_MEMBERS_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_40
#define YB_GEN_COL_MEMBERS_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_40
#define YB_GEN_COL_MEMBERS_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_40
#define YB_GEN_COL_MEMBERS_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_39
#define YB_GEN_COL_MEMBERS_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_39
#define YB_GEN_COL_MEMBERS_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_39
#define YB_GEN_COL_MEMBERS_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_38
#define YB_GEN_COL_MEMBERS_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_38
#define YB_GEN_COL_MEMBERS_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_38
#define YB_GEN_COL_MEMBERS_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_37
#define YB_GEN_COL_MEMBERS_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_37
#define YB_GEN_COL_MEMBERS_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_37
#define YB_GEN_COL_MEMBERS_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_36
#define YB_GEN_COL_MEMBERS_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_36
#define YB_GEN_COL_MEMBERS_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_36
#define YB_GEN_COL_MEMBERS_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_35
#define YB_GEN_COL_MEMBERS_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_35
#define YB_GEN_COL_MEMBERS_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_35
#define YB_GEN_COL_MEMBERS_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_34
#define YB_GEN_COL_MEMBERS_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_34
#define YB_GEN_COL_MEMBERS_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_34
#define YB_GEN_COL_MEMBERS_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_33
#define YB_GEN_COL_MEMBERS_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_33
#define YB_GEN_COL_MEMBERS_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_33
#define YB_GEN_COL_MEMBERS_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_32
#define YB_GEN_COL_MEMBERS_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_32
#define YB_GEN_COL_MEMBERS_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_32
#define YB_GEN_COL_MEMBERS_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_31
#define YB_GEN_COL_MEMBERS_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_31
#define YB_GEN_COL_MEMBERS_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_31
#define YB_GEN_COL_MEMBERS_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_30
#define YB_GEN_COL_MEMBERS_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_30
#define YB_GEN_COL_MEMBERS_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_30
#define YB_GEN_COL_MEMBERS_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_29
#define YB_GEN_COL_MEMBERS_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_29
#define YB_GEN_COL_MEMBERS_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_29
#define YB_GEN_COL_MEMBERS_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_28
#define YB_GEN_COL_MEMBERS_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_28
#define YB_GEN_COL_MEMBERS_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_28
#define YB_GEN_COL_MEMBERS_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_27
#define YB_GEN_COL_MEMBERS_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_27
#define YB_GEN_COL_MEMBERS_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_27
#define YB_GEN_COL_MEMBERS_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_26
#define YB_GEN_COL_MEMBERS_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_26
#define YB_GEN_COL_MEMBERS_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_26
#define YB_GEN_COL_MEMBERS_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_25
#define YB_GEN_COL_MEMBERS_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_25
#define YB_GEN_COL_MEMBERS_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_25
#define YB_GEN_COL_MEMBERS_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_24
#define YB_GEN_COL_MEMBERS_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_24
#define YB_GEN_COL_MEMBERS_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_24
#define YB_GEN_COL_MEMBERS_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_23
#define YB_GEN_COL_MEMBERS_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_23
#define YB_GEN_COL_MEMBERS_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_23
#define YB_GEN_COL_MEMBERS_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_22
#define YB_GEN_COL_MEMBERS_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_22
#define YB_GEN_COL_MEMBERS_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_22
#define YB_GEN_COL_MEMBERS_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_21
#define YB_GEN_COL_MEMBERS_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_21
#define YB_GEN_COL_MEMBERS_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_21
#define YB_GEN_COL_MEMBERS_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_20
#define YB_GEN_COL_MEMBERS_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_20
#define YB_GEN_COL_MEMBERS_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_20
#define YB_GEN_COL_MEMBERS_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_19
#define YB_GEN_COL_MEMBERS_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_19
#define YB_GEN_COL_MEMBERS_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_19
#define YB_GEN_COL_MEMBERS_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_18
#define YB_GEN_COL_MEMBERS_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_18
#define YB_GEN_COL_MEMBERS_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_18
#define YB_GEN_COL_MEMBERS_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_17
#define YB_GEN_COL_MEMBERS_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_17
#define YB_GEN_COL_MEMBERS_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_17
#define YB_GEN_COL_MEMBERS_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_16
#define YB_GEN_COL_MEMBERS_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_16
#define YB_GEN_COL_MEMBERS_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_16
#define YB_GEN_COL_MEMBERS_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_15
#define YB_GEN_COL_MEMBERS_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_15
#define YB_GEN_COL_MEMBERS_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_15
#define YB_GEN_COL_MEMBERS_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_14
#define YB_GEN_COL_MEMBERS_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_14
#define YB_GEN_COL_MEMBERS_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_14
#define YB_GEN_COL_MEMBERS_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_13
#define YB_GEN_COL_MEMBERS_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_13
#define YB_GEN_COL_MEMBERS_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_13
#define YB_GEN_COL_MEMBERS_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_12
#define YB_GEN_COL_MEMBERS_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_12
#define YB_GEN_COL_MEMBERS_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_12
#define YB_GEN_COL_MEMBERS_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_11
#define YB_GEN_COL_MEMBERS_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_11
#define YB_GEN_COL_MEMBERS_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_11
#define YB_GEN_COL_MEMBERS_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_10
#define YB_GEN_COL_MEMBERS_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_10
#define YB_GEN_COL_MEMBERS_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_10
#define YB_GEN_COL_MEMBERS_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_9
#define YB_GEN_COL_MEMBERS_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_9
#define YB_GEN_COL_MEMBERS_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_9
#define YB_GEN_COL_MEMBERS_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_8
#define YB_GEN_COL_MEMBERS_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_8
#define YB_GEN_COL_MEMBERS_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_8
#define YB_GEN_COL_MEMBERS_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_7
#define YB_GEN_COL_MEMBERS_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_7
#define YB_GEN_COL_MEMBERS_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_7
#define YB_GEN_COL_MEMBERS_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_6
#define YB_GEN_COL_MEMBERS_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_6
#define YB_GEN_COL_MEMBERS_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_6
#define YB_GEN_COL_MEMBERS_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_5
#define YB_GEN_COL_MEMBERS_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_5
#define YB_GEN_COL_MEMBERS_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_5
#define YB_GEN_COL_MEMBERS_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_4
#define YB_GEN_COL_MEMBERS_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_4
#define YB_GEN_COL_MEMBERS_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_4
#define YB_GEN_COL_MEMBERS_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_3
#define YB_GEN_COL_MEMBERS_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_3
#define YB_GEN_COL_MEMBERS_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_3
#define YB_GEN_COL_MEMBERS_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_2
#define YB_GEN_COL_MEMBERS_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_2
#define YB_GEN_COL_MEMBERS_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_2
#define YB_GEN_COL_MEMBERS_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMBERS_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_1
#define YB_GEN_COL_MEMBERS_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_1
#define YB_GEN_COL_MEMBERS_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMBERS_1
#define YB_GEN_COL_MEMBERS_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMBERS(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMBERS_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_42
#define YB_GEN_COL_ADD_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_42
#define YB_GEN_COL_ADD_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_42
#define YB_GEN_COL_ADD_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_41
#define YB_GEN_COL_ADD_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_41
#define YB_GEN_COL_ADD_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_41
#define YB_GEN_COL_ADD_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_40
#define YB_GEN_COL_ADD_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_40
#define YB_GEN_COL_ADD_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_40
#define YB_GEN_COL_ADD_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_39
#define YB_GEN_COL_ADD_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_39
#define YB_GEN_COL_ADD_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_39
#define YB_GEN_COL_ADD_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_38
#define YB_GEN_COL_ADD_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_38
#define YB_GEN_COL_ADD_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_38
#define YB_GEN_COL_ADD_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_37
#define YB_GEN_COL_ADD_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_37
#define YB_GEN_COL_ADD_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_37
#define YB_GEN_COL_ADD_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_36
#define YB_GEN_COL_ADD_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_36
#define YB_GEN_COL_ADD_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_36
#define YB_GEN_COL_ADD_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_35
#define YB_GEN_COL_ADD_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_35
#define YB_GEN_COL_ADD_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_35
#define YB_GEN_COL_ADD_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_34
#define YB_GEN_COL_ADD_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_34
#define YB_GEN_COL_ADD_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_34
#define YB_GEN_COL_ADD_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_33
#define YB_GEN_COL_ADD_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_33
#define YB_GEN_COL_ADD_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_33
#define YB_GEN_COL_ADD_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_32
#define YB_GEN_COL_ADD_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_32
#define YB_GEN_COL_ADD_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_32
#define YB_GEN_COL_ADD_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_31
#define YB_GEN_COL_ADD_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_31
#define YB_GEN_COL_ADD_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_31
#define YB_GEN_COL_ADD_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_30
#define YB_GEN_COL_ADD_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_30
#define YB_GEN_COL_ADD_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_30
#define YB_GEN_COL_ADD_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_29
#define YB_GEN_COL_ADD_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_29
#define YB_GEN_COL_ADD_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_29
#define YB_GEN_COL_ADD_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_28
#define YB_GEN_COL_ADD_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_28
#define YB_GEN_COL_ADD_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_28
#define YB_GEN_COL_ADD_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_27
#define YB_GEN_COL_ADD_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_27
#define YB_GEN_COL_ADD_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_27
#define YB_GEN_COL_ADD_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_26
#define YB_GEN_COL_ADD_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_26
#define YB_GEN_COL_ADD_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_26
#define YB_GEN_COL_ADD_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_25
#define YB_GEN_COL_ADD_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_25
#define YB_GEN_COL_ADD_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_25
#define YB_GEN_COL_ADD_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_24
#define YB_GEN_COL_ADD_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_24
#define YB_GEN_COL_ADD_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_24
#define YB_GEN_COL_ADD_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_23
#define YB_GEN_COL_ADD_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_23
#define YB_GEN_COL_ADD_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_23
#define YB_GEN_COL_ADD_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_22
#define YB_GEN_COL_ADD_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_22
#define YB_GEN_COL_ADD_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_22
#define YB_GEN_COL_ADD_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_21
#define YB_GEN_COL_ADD_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_21
#define YB_GEN_COL_ADD_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_21
#define YB_GEN_COL_ADD_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_20
#define YB_GEN_COL_ADD_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_20
#define YB_GEN_COL_ADD_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_20
#define YB_GEN_COL_ADD_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_19
#define YB_GEN_COL_ADD_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_19
#define YB_GEN_COL_ADD_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_19
#define YB_GEN_COL_ADD_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_18
#define YB_GEN_COL_ADD_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_18
#define YB_GEN_COL_ADD_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_18
#define YB_GEN_COL_ADD_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_17
#define YB_GEN_COL_ADD_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_17
#define YB_GEN_COL_ADD_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_17
#define YB_GEN_COL_ADD_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_16
#define YB_GEN_COL_ADD_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_16
#define YB_GEN_COL_ADD_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_16
#define YB_GEN_COL_ADD_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_15
#define YB_GEN_COL_ADD_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_15
#define YB_GEN_COL_ADD_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_15
#define YB_GEN_COL_ADD_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_14
#define YB_GEN_COL_ADD_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_14
#define YB_GEN_COL_ADD_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_14
#define YB_GEN_COL_ADD_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_13
#define YB_GEN_COL_ADD_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_13
#define YB_GEN_COL_ADD_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_13
#define YB_GEN_COL_ADD_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_12
#define YB_GEN_COL_ADD_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_12
#define YB_GEN_COL_ADD_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_12
#define YB_GEN_COL_ADD_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_11
#define YB_GEN_COL_ADD_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_11
#define YB_GEN_COL_ADD_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_11
#define YB_GEN_COL_ADD_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_10
#define YB_GEN_COL_ADD_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_10
#define YB_GEN_COL_ADD_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_10
#define YB_GEN_COL_ADD_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_9
#define YB_GEN_COL_ADD_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_9
#define YB_GEN_COL_ADD_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_9
#define YB_GEN_COL_ADD_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_8
#define YB_GEN_COL_ADD_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_8
#define YB_GEN_COL_ADD_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_8
#define YB_GEN_COL_ADD_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_7
#define YB_GEN_COL_ADD_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_7
#define YB_GEN_COL_ADD_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_7
#define YB_GEN_COL_ADD_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_6
#define YB_GEN_COL_ADD_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_6
#define YB_GEN_COL_ADD_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_6
#define YB_GEN_COL_ADD_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_5
#define YB_GEN_COL_ADD_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_5
#define YB_GEN_COL_ADD_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_5
#define YB_GEN_COL_ADD_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_4
#define YB_GEN_COL_ADD_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_4
#define YB_GEN_COL_ADD_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_4
#define YB_GEN_COL_ADD_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_3
#define YB_GEN_COL_ADD_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_3
#define YB_GEN_COL_ADD_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_3
#define YB_GEN_COL_ADD_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_2
#define YB_GEN_COL_ADD_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_2
#define YB_GEN_COL_ADD_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_2
#define YB_GEN_COL_ADD_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_ADD_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_1
#define YB_GEN_COL_ADD_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_1
#define YB_GEN_COL_ADD_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_ADD_1
#define YB_GEN_COL_ADD_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_ADD(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_ADD_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_42
#define YB_GEN_COL_MEMB_CONS_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_42
#define YB_GEN_COL_MEMB_CONS_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_42
#define YB_GEN_COL_MEMB_CONS_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_41
#define YB_GEN_COL_MEMB_CONS_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_41
#define YB_GEN_COL_MEMB_CONS_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_41
#define YB_GEN_COL_MEMB_CONS_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_40
#define YB_GEN_COL_MEMB_CONS_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_40
#define YB_GEN_COL_MEMB_CONS_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_40
#define YB_GEN_COL_MEMB_CONS_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_39
#define YB_GEN_COL_MEMB_CONS_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_39
#define YB_GEN_COL_MEMB_CONS_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_39
#define YB_GEN_COL_MEMB_CONS_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_38
#define YB_GEN_COL_MEMB_CONS_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_38
#define YB_GEN_COL_MEMB_CONS_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_38
#define YB_GEN_COL_MEMB_CONS_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_37
#define YB_GEN_COL_MEMB_CONS_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_37
#define YB_GEN_COL_MEMB_CONS_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_37
#define YB_GEN_COL_MEMB_CONS_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_36
#define YB_GEN_COL_MEMB_CONS_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_36
#define YB_GEN_COL_MEMB_CONS_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_36
#define YB_GEN_COL_MEMB_CONS_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_35
#define YB_GEN_COL_MEMB_CONS_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_35
#define YB_GEN_COL_MEMB_CONS_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_35
#define YB_GEN_COL_MEMB_CONS_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_34
#define YB_GEN_COL_MEMB_CONS_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_34
#define YB_GEN_COL_MEMB_CONS_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_34
#define YB_GEN_COL_MEMB_CONS_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_33
#define YB_GEN_COL_MEMB_CONS_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_33
#define YB_GEN_COL_MEMB_CONS_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_33
#define YB_GEN_COL_MEMB_CONS_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_32
#define YB_GEN_COL_MEMB_CONS_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_32
#define YB_GEN_COL_MEMB_CONS_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_32
#define YB_GEN_COL_MEMB_CONS_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_31
#define YB_GEN_COL_MEMB_CONS_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_31
#define YB_GEN_COL_MEMB_CONS_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_31
#define YB_GEN_COL_MEMB_CONS_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_30
#define YB_GEN_COL_MEMB_CONS_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_30
#define YB_GEN_COL_MEMB_CONS_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_30
#define YB_GEN_COL_MEMB_CONS_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_29
#define YB_GEN_COL_MEMB_CONS_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_29
#define YB_GEN_COL_MEMB_CONS_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_29
#define YB_GEN_COL_MEMB_CONS_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_28
#define YB_GEN_COL_MEMB_CONS_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_28
#define YB_GEN_COL_MEMB_CONS_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_28
#define YB_GEN_COL_MEMB_CONS_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_27
#define YB_GEN_COL_MEMB_CONS_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_27
#define YB_GEN_COL_MEMB_CONS_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_27
#define YB_GEN_COL_MEMB_CONS_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_26
#define YB_GEN_COL_MEMB_CONS_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_26
#define YB_GEN_COL_MEMB_CONS_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_26
#define YB_GEN_COL_MEMB_CONS_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_25
#define YB_GEN_COL_MEMB_CONS_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_25
#define YB_GEN_COL_MEMB_CONS_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_25
#define YB_GEN_COL_MEMB_CONS_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_24
#define YB_GEN_COL_MEMB_CONS_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_24
#define YB_GEN_COL_MEMB_CONS_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_24
#define YB_GEN_COL_MEMB_CONS_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_23
#define YB_GEN_COL_MEMB_CONS_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_23
#define YB_GEN_COL_MEMB_CONS_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_23
#define YB_GEN_COL_MEMB_CONS_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_22
#define YB_GEN_COL_MEMB_CONS_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_22
#define YB_GEN_COL_MEMB_CONS_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_22
#define YB_GEN_COL_MEMB_CONS_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_21
#define YB_GEN_COL_MEMB_CONS_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_21
#define YB_GEN_COL_MEMB_CONS_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_21
#define YB_GEN_COL_MEMB_CONS_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_20
#define YB_GEN_COL_MEMB_CONS_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_20
#define YB_GEN_COL_MEMB_CONS_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_20
#define YB_GEN_COL_MEMB_CONS_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_19
#define YB_GEN_COL_MEMB_CONS_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_19
#define YB_GEN_COL_MEMB_CONS_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_19
#define YB_GEN_COL_MEMB_CONS_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_18
#define YB_GEN_COL_MEMB_CONS_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_18
#define YB_GEN_COL_MEMB_CONS_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_18
#define YB_GEN_COL_MEMB_CONS_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_17
#define YB_GEN_COL_MEMB_CONS_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_17
#define YB_GEN_COL_MEMB_CONS_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_17
#define YB_GEN_COL_MEMB_CONS_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_16
#define YB_GEN_COL_MEMB_CONS_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_16
#define YB_GEN_COL_MEMB_CONS_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_16
#define YB_GEN_COL_MEMB_CONS_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_15
#define YB_GEN_COL_MEMB_CONS_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_15
#define YB_GEN_COL_MEMB_CONS_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_15
#define YB_GEN_COL_MEMB_CONS_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_14
#define YB_GEN_COL_MEMB_CONS_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_14
#define YB_GEN_COL_MEMB_CONS_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_14
#define YB_GEN_COL_MEMB_CONS_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_13
#define YB_GEN_COL_MEMB_CONS_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_13
#define YB_GEN_COL_MEMB_CONS_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_13
#define YB_GEN_COL_MEMB_CONS_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_12
#define YB_GEN_COL_MEMB_CONS_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_12
#define YB_GEN_COL_MEMB_CONS_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_12
#define YB_GEN_COL_MEMB_CONS_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_11
#define YB_GEN_COL_MEMB_CONS_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_11
#define YB_GEN_COL_MEMB_CONS_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_11
#define YB_GEN_COL_MEMB_CONS_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_10
#define YB_GEN_COL_MEMB_CONS_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_10
#define YB_GEN_COL_MEMB_CONS_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_10
#define YB_GEN_COL_MEMB_CONS_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_9
#define YB_GEN_COL_MEMB_CONS_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_9
#define YB_GEN_COL_MEMB_CONS_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_9
#define YB_GEN_COL_MEMB_CONS_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_8
#define YB_GEN_COL_MEMB_CONS_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_8
#define YB_GEN_COL_MEMB_CONS_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_8
#define YB_GEN_COL_MEMB_CONS_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_7
#define YB_GEN_COL_MEMB_CONS_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_7
#define YB_GEN_COL_MEMB_CONS_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_7
#define YB_GEN_COL_MEMB_CONS_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_6
#define YB_GEN_COL_MEMB_CONS_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_6
#define YB_GEN_COL_MEMB_CONS_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_6
#define YB_GEN_COL_MEMB_CONS_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_5
#define YB_GEN_COL_MEMB_CONS_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_5
#define YB_GEN_COL_MEMB_CONS_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_5
#define YB_GEN_COL_MEMB_CONS_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_4
#define YB_GEN_COL_MEMB_CONS_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_4
#define YB_GEN_COL_MEMB_CONS_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_4
#define YB_GEN_COL_MEMB_CONS_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_3
#define YB_GEN_COL_MEMB_CONS_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_3
#define YB_GEN_COL_MEMB_CONS_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_3
#define YB_GEN_COL_MEMB_CONS_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_2
#define YB_GEN_COL_MEMB_CONS_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_2
#define YB_GEN_COL_MEMB_CONS_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_2
#define YB_GEN_COL_MEMB_CONS_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_COL_MEMB_CONS_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_1
#define YB_GEN_COL_MEMB_CONS_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_1
#define YB_GEN_COL_MEMB_CONS_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_COL_MEMB_CONS_1
#define YB_GEN_COL_MEMB_CONS_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_COL_MEMB_CONS(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_COL_MEMB_CONS_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_42
#define YB_GEN_PROP_DECL_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_42
#define YB_GEN_PROP_DECL_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_42
#define YB_GEN_PROP_DECL_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_41
#define YB_GEN_PROP_DECL_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_41
#define YB_GEN_PROP_DECL_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_41
#define YB_GEN_PROP_DECL_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_40
#define YB_GEN_PROP_DECL_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_40
#define YB_GEN_PROP_DECL_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_40
#define YB_GEN_PROP_DECL_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_39
#define YB_GEN_PROP_DECL_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_39
#define YB_GEN_PROP_DECL_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_39
#define YB_GEN_PROP_DECL_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_38
#define YB_GEN_PROP_DECL_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_38
#define YB_GEN_PROP_DECL_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_38
#define YB_GEN_PROP_DECL_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_37
#define YB_GEN_PROP_DECL_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_37
#define YB_GEN_PROP_DECL_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_37
#define YB_GEN_PROP_DECL_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_36
#define YB_GEN_PROP_DECL_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_36
#define YB_GEN_PROP_DECL_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_36
#define YB_GEN_PROP_DECL_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_35
#define YB_GEN_PROP_DECL_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_35
#define YB_GEN_PROP_DECL_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_35
#define YB_GEN_PROP_DECL_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_34
#define YB_GEN_PROP_DECL_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_34
#define YB_GEN_PROP_DECL_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_34
#define YB_GEN_PROP_DECL_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_33
#define YB_GEN_PROP_DECL_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_33
#define YB_GEN_PROP_DECL_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_33
#define YB_GEN_PROP_DECL_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_32
#define YB_GEN_PROP_DECL_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_32
#define YB_GEN_PROP_DECL_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_32
#define YB_GEN_PROP_DECL_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_31
#define YB_GEN_PROP_DECL_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_31
#define YB_GEN_PROP_DECL_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_31
#define YB_GEN_PROP_DECL_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_30
#define YB_GEN_PROP_DECL_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_30
#define YB_GEN_PROP_DECL_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_30
#define YB_GEN_PROP_DECL_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_29
#define YB_GEN_PROP_DECL_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_29
#define YB_GEN_PROP_DECL_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_29
#define YB_GEN_PROP_DECL_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_28
#define YB_GEN_PROP_DECL_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_28
#define YB_GEN_PROP_DECL_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_28
#define YB_GEN_PROP_DECL_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_27
#define YB_GEN_PROP_DECL_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_27
#define YB_GEN_PROP_DECL_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_27
#define YB_GEN_PROP_DECL_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_26
#define YB_GEN_PROP_DECL_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_26
#define YB_GEN_PROP_DECL_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_26
#define YB_GEN_PROP_DECL_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_25
#define YB_GEN_PROP_DECL_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_25
#define YB_GEN_PROP_DECL_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_25
#define YB_GEN_PROP_DECL_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_24
#define YB_GEN_PROP_DECL_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_24
#define YB_GEN_PROP_DECL_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_24
#define YB_GEN_PROP_DECL_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_23
#define YB_GEN_PROP_DECL_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_23
#define YB_GEN_PROP_DECL_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_23
#define YB_GEN_PROP_DECL_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_22
#define YB_GEN_PROP_DECL_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_22
#define YB_GEN_PROP_DECL_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_22
#define YB_GEN_PROP_DECL_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_21
#define YB_GEN_PROP_DECL_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_21
#define YB_GEN_PROP_DECL_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_21
#define YB_GEN_PROP_DECL_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_20
#define YB_GEN_PROP_DECL_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_20
#define YB_GEN_PROP_DECL_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_20
#define YB_GEN_PROP_DECL_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_19
#define YB_GEN_PROP_DECL_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_19
#define YB_GEN_PROP_DECL_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_19
#define YB_GEN_PROP_DECL_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_18
#define YB_GEN_PROP_DECL_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_18
#define YB_GEN_PROP_DECL_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_18
#define YB_GEN_PROP_DECL_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_17
#define YB_GEN_PROP_DECL_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_17
#define YB_GEN_PROP_DECL_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_17
#define YB_GEN_PROP_DECL_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_16
#define YB_GEN_PROP_DECL_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_16
#define YB_GEN_PROP_DECL_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_16
#define YB_GEN_PROP_DECL_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_15
#define YB_GEN_PROP_DECL_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_15
#define YB_GEN_PROP_DECL_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_15
#define YB_GEN_PROP_DECL_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_14
#define YB_GEN_PROP_DECL_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_14
#define YB_GEN_PROP_DECL_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_14
#define YB_GEN_PROP_DECL_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_13
#define YB_GEN_PROP_DECL_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_13
#define YB_GEN_PROP_DECL_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_13
#define YB_GEN_PROP_DECL_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_12
#define YB_GEN_PROP_DECL_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_12
#define YB_GEN_PROP_DECL_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_12
#define YB_GEN_PROP_DECL_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_11
#define YB_GEN_PROP_DECL_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_11
#define YB_GEN_PROP_DECL_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_11
#define YB_GEN_PROP_DECL_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_10
#define YB_GEN_PROP_DECL_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_10
#define YB_GEN_PROP_DECL_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_10
#define YB_GEN_PROP_DECL_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_9
#define YB_GEN_PROP_DECL_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_9
#define YB_GEN_PROP_DECL_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_9
#define YB_GEN_PROP_DECL_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_8
#define YB_GEN_PROP_DECL_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_8
#define YB_GEN_PROP_DECL_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_8
#define YB_GEN_PROP_DECL_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_7
#define YB_GEN_PROP_DECL_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_7
#define YB_GEN_PROP_DECL_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_7
#define YB_GEN_PROP_DECL_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_6
#define YB_GEN_PROP_DECL_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_6
#define YB_GEN_PROP_DECL_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_6
#define YB_GEN_PROP_DECL_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_5
#define YB_GEN_PROP_DECL_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_5
#define YB_GEN_PROP_DECL_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_5
#define YB_GEN_PROP_DECL_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_4
#define YB_GEN_PROP_DECL_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_4
#define YB_GEN_PROP_DECL_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_4
#define YB_GEN_PROP_DECL_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_3
#define YB_GEN_PROP_DECL_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_3
#define YB_GEN_PROP_DECL_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_3
#define YB_GEN_PROP_DECL_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_2
#define YB_GEN_PROP_DECL_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_2
#define YB_GEN_PROP_DECL_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_2
#define YB_GEN_PROP_DECL_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DECL_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_MANY_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_1
#define YB_GEN_PROP_DECL_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_REL_ONE_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_1
#define YB_GEN_PROP_DECL_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DECL_1
#define YB_GEN_PROP_DECL_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DECL(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DECL_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_42
#define YB_GEN_PROP_CONS_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_42
#define YB_GEN_PROP_CONS_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_42
#define YB_GEN_PROP_CONS_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_41
#define YB_GEN_PROP_CONS_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_41
#define YB_GEN_PROP_CONS_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_41
#define YB_GEN_PROP_CONS_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_40
#define YB_GEN_PROP_CONS_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_40
#define YB_GEN_PROP_CONS_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_40
#define YB_GEN_PROP_CONS_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_39
#define YB_GEN_PROP_CONS_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_39
#define YB_GEN_PROP_CONS_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_39
#define YB_GEN_PROP_CONS_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_38
#define YB_GEN_PROP_CONS_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_38
#define YB_GEN_PROP_CONS_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_38
#define YB_GEN_PROP_CONS_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_37
#define YB_GEN_PROP_CONS_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_37
#define YB_GEN_PROP_CONS_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_37
#define YB_GEN_PROP_CONS_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_36
#define YB_GEN_PROP_CONS_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_36
#define YB_GEN_PROP_CONS_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_36
#define YB_GEN_PROP_CONS_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_35
#define YB_GEN_PROP_CONS_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_35
#define YB_GEN_PROP_CONS_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_35
#define YB_GEN_PROP_CONS_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_34
#define YB_GEN_PROP_CONS_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_34
#define YB_GEN_PROP_CONS_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_34
#define YB_GEN_PROP_CONS_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_33
#define YB_GEN_PROP_CONS_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_33
#define YB_GEN_PROP_CONS_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_33
#define YB_GEN_PROP_CONS_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_32
#define YB_GEN_PROP_CONS_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_32
#define YB_GEN_PROP_CONS_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_32
#define YB_GEN_PROP_CONS_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_31
#define YB_GEN_PROP_CONS_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_31
#define YB_GEN_PROP_CONS_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_31
#define YB_GEN_PROP_CONS_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_30
#define YB_GEN_PROP_CONS_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_30
#define YB_GEN_PROP_CONS_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_30
#define YB_GEN_PROP_CONS_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_29
#define YB_GEN_PROP_CONS_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_29
#define YB_GEN_PROP_CONS_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_29
#define YB_GEN_PROP_CONS_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_28
#define YB_GEN_PROP_CONS_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_28
#define YB_GEN_PROP_CONS_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_28
#define YB_GEN_PROP_CONS_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_27
#define YB_GEN_PROP_CONS_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_27
#define YB_GEN_PROP_CONS_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_27
#define YB_GEN_PROP_CONS_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_26
#define YB_GEN_PROP_CONS_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_26
#define YB_GEN_PROP_CONS_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_26
#define YB_GEN_PROP_CONS_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_25
#define YB_GEN_PROP_CONS_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_25
#define YB_GEN_PROP_CONS_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_25
#define YB_GEN_PROP_CONS_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_24
#define YB_GEN_PROP_CONS_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_24
#define YB_GEN_PROP_CONS_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_24
#define YB_GEN_PROP_CONS_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_23
#define YB_GEN_PROP_CONS_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_23
#define YB_GEN_PROP_CONS_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_23
#define YB_GEN_PROP_CONS_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_22
#define YB_GEN_PROP_CONS_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_22
#define YB_GEN_PROP_CONS_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_22
#define YB_GEN_PROP_CONS_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_21
#define YB_GEN_PROP_CONS_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_21
#define YB_GEN_PROP_CONS_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_21
#define YB_GEN_PROP_CONS_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_20
#define YB_GEN_PROP_CONS_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_20
#define YB_GEN_PROP_CONS_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_20
#define YB_GEN_PROP_CONS_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_19
#define YB_GEN_PROP_CONS_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_19
#define YB_GEN_PROP_CONS_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_19
#define YB_GEN_PROP_CONS_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_18
#define YB_GEN_PROP_CONS_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_18
#define YB_GEN_PROP_CONS_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_18
#define YB_GEN_PROP_CONS_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_17
#define YB_GEN_PROP_CONS_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_17
#define YB_GEN_PROP_CONS_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_17
#define YB_GEN_PROP_CONS_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_16
#define YB_GEN_PROP_CONS_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_16
#define YB_GEN_PROP_CONS_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_16
#define YB_GEN_PROP_CONS_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_15
#define YB_GEN_PROP_CONS_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_15
#define YB_GEN_PROP_CONS_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_15
#define YB_GEN_PROP_CONS_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_14
#define YB_GEN_PROP_CONS_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_14
#define YB_GEN_PROP_CONS_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_14
#define YB_GEN_PROP_CONS_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_13
#define YB_GEN_PROP_CONS_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_13
#define YB_GEN_PROP_CONS_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_13
#define YB_GEN_PROP_CONS_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_12
#define YB_GEN_PROP_CONS_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_12
#define YB_GEN_PROP_CONS_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_12
#define YB_GEN_PROP_CONS_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_11
#define YB_GEN_PROP_CONS_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_11
#define YB_GEN_PROP_CONS_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_11
#define YB_GEN_PROP_CONS_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_10
#define YB_GEN_PROP_CONS_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_10
#define YB_GEN_PROP_CONS_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_10
#define YB_GEN_PROP_CONS_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_9
#define YB_GEN_PROP_CONS_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_9
#define YB_GEN_PROP_CONS_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_9
#define YB_GEN_PROP_CONS_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_8
#define YB_GEN_PROP_CONS_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_8
#define YB_GEN_PROP_CONS_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_8
#define YB_GEN_PROP_CONS_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_7
#define YB_GEN_PROP_CONS_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_7
#define YB_GEN_PROP_CONS_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_7
#define YB_GEN_PROP_CONS_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_6
#define YB_GEN_PROP_CONS_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_6
#define YB_GEN_PROP_CONS_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_6
#define YB_GEN_PROP_CONS_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_5
#define YB_GEN_PROP_CONS_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_5
#define YB_GEN_PROP_CONS_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_5
#define YB_GEN_PROP_CONS_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_4
#define YB_GEN_PROP_CONS_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_4
#define YB_GEN_PROP_CONS_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_4
#define YB_GEN_PROP_CONS_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_3
#define YB_GEN_PROP_CONS_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_3
#define YB_GEN_PROP_CONS_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_3
#define YB_GEN_PROP_CONS_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_2
#define YB_GEN_PROP_CONS_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_2
#define YB_GEN_PROP_CONS_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_2
#define YB_GEN_PROP_CONS_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_CONS_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_MANY_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_1
#define YB_GEN_PROP_CONS_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_REL_ONE_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_1
#define YB_GEN_PROP_CONS_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_CONS_1
#define YB_GEN_PROP_CONS_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_CONS(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_CONS_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_42
#define YB_GEN_PROP_DEFAULT_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_42
#define YB_GEN_PROP_DEFAULT_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_42
#define YB_GEN_PROP_DEFAULT_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_41
#define YB_GEN_PROP_DEFAULT_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_41
#define YB_GEN_PROP_DEFAULT_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_41
#define YB_GEN_PROP_DEFAULT_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_40
#define YB_GEN_PROP_DEFAULT_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_40
#define YB_GEN_PROP_DEFAULT_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_40
#define YB_GEN_PROP_DEFAULT_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_39
#define YB_GEN_PROP_DEFAULT_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_39
#define YB_GEN_PROP_DEFAULT_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_39
#define YB_GEN_PROP_DEFAULT_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_38
#define YB_GEN_PROP_DEFAULT_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_38
#define YB_GEN_PROP_DEFAULT_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_38
#define YB_GEN_PROP_DEFAULT_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_37
#define YB_GEN_PROP_DEFAULT_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_37
#define YB_GEN_PROP_DEFAULT_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_37
#define YB_GEN_PROP_DEFAULT_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_36
#define YB_GEN_PROP_DEFAULT_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_36
#define YB_GEN_PROP_DEFAULT_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_36
#define YB_GEN_PROP_DEFAULT_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_35
#define YB_GEN_PROP_DEFAULT_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_35
#define YB_GEN_PROP_DEFAULT_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_35
#define YB_GEN_PROP_DEFAULT_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_34
#define YB_GEN_PROP_DEFAULT_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_34
#define YB_GEN_PROP_DEFAULT_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_34
#define YB_GEN_PROP_DEFAULT_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_33
#define YB_GEN_PROP_DEFAULT_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_33
#define YB_GEN_PROP_DEFAULT_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_33
#define YB_GEN_PROP_DEFAULT_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_32
#define YB_GEN_PROP_DEFAULT_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_32
#define YB_GEN_PROP_DEFAULT_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_32
#define YB_GEN_PROP_DEFAULT_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_31
#define YB_GEN_PROP_DEFAULT_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_31
#define YB_GEN_PROP_DEFAULT_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_31
#define YB_GEN_PROP_DEFAULT_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_30
#define YB_GEN_PROP_DEFAULT_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_30
#define YB_GEN_PROP_DEFAULT_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_30
#define YB_GEN_PROP_DEFAULT_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_29
#define YB_GEN_PROP_DEFAULT_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_29
#define YB_GEN_PROP_DEFAULT_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_29
#define YB_GEN_PROP_DEFAULT_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_28
#define YB_GEN_PROP_DEFAULT_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_28
#define YB_GEN_PROP_DEFAULT_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_28
#define YB_GEN_PROP_DEFAULT_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_27
#define YB_GEN_PROP_DEFAULT_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_27
#define YB_GEN_PROP_DEFAULT_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_27
#define YB_GEN_PROP_DEFAULT_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_26
#define YB_GEN_PROP_DEFAULT_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_26
#define YB_GEN_PROP_DEFAULT_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_26
#define YB_GEN_PROP_DEFAULT_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_25
#define YB_GEN_PROP_DEFAULT_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_25
#define YB_GEN_PROP_DEFAULT_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_25
#define YB_GEN_PROP_DEFAULT_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_24
#define YB_GEN_PROP_DEFAULT_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_24
#define YB_GEN_PROP_DEFAULT_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_24
#define YB_GEN_PROP_DEFAULT_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_23
#define YB_GEN_PROP_DEFAULT_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_23
#define YB_GEN_PROP_DEFAULT_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_23
#define YB_GEN_PROP_DEFAULT_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_22
#define YB_GEN_PROP_DEFAULT_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_22
#define YB_GEN_PROP_DEFAULT_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_22
#define YB_GEN_PROP_DEFAULT_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_21
#define YB_GEN_PROP_DEFAULT_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_21
#define YB_GEN_PROP_DEFAULT_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_21
#define YB_GEN_PROP_DEFAULT_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_20
#define YB_GEN_PROP_DEFAULT_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_20
#define YB_GEN_PROP_DEFAULT_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_20
#define YB_GEN_PROP_DEFAULT_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_19
#define YB_GEN_PROP_DEFAULT_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_19
#define YB_GEN_PROP_DEFAULT_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_19
#define YB_GEN_PROP_DEFAULT_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_18
#define YB_GEN_PROP_DEFAULT_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_18
#define YB_GEN_PROP_DEFAULT_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_18
#define YB_GEN_PROP_DEFAULT_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_17
#define YB_GEN_PROP_DEFAULT_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_17
#define YB_GEN_PROP_DEFAULT_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_17
#define YB_GEN_PROP_DEFAULT_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_16
#define YB_GEN_PROP_DEFAULT_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_16
#define YB_GEN_PROP_DEFAULT_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_16
#define YB_GEN_PROP_DEFAULT_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_15
#define YB_GEN_PROP_DEFAULT_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_15
#define YB_GEN_PROP_DEFAULT_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_15
#define YB_GEN_PROP_DEFAULT_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_14
#define YB_GEN_PROP_DEFAULT_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_14
#define YB_GEN_PROP_DEFAULT_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_14
#define YB_GEN_PROP_DEFAULT_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_13
#define YB_GEN_PROP_DEFAULT_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_13
#define YB_GEN_PROP_DEFAULT_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_13
#define YB_GEN_PROP_DEFAULT_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_12
#define YB_GEN_PROP_DEFAULT_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_12
#define YB_GEN_PROP_DEFAULT_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_12
#define YB_GEN_PROP_DEFAULT_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_11
#define YB_GEN_PROP_DEFAULT_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_11
#define YB_GEN_PROP_DEFAULT_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_11
#define YB_GEN_PROP_DEFAULT_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_10
#define YB_GEN_PROP_DEFAULT_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_10
#define YB_GEN_PROP_DEFAULT_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_10
#define YB_GEN_PROP_DEFAULT_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_9
#define YB_GEN_PROP_DEFAULT_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_9
#define YB_GEN_PROP_DEFAULT_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_9
#define YB_GEN_PROP_DEFAULT_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_8
#define YB_GEN_PROP_DEFAULT_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_8
#define YB_GEN_PROP_DEFAULT_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_8
#define YB_GEN_PROP_DEFAULT_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_7
#define YB_GEN_PROP_DEFAULT_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_7
#define YB_GEN_PROP_DEFAULT_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_7
#define YB_GEN_PROP_DEFAULT_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_6
#define YB_GEN_PROP_DEFAULT_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_6
#define YB_GEN_PROP_DEFAULT_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_6
#define YB_GEN_PROP_DEFAULT_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_5
#define YB_GEN_PROP_DEFAULT_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_5
#define YB_GEN_PROP_DEFAULT_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_5
#define YB_GEN_PROP_DEFAULT_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_4
#define YB_GEN_PROP_DEFAULT_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_4
#define YB_GEN_PROP_DEFAULT_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_4
#define YB_GEN_PROP_DEFAULT_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_3
#define YB_GEN_PROP_DEFAULT_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_3
#define YB_GEN_PROP_DEFAULT_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_3
#define YB_GEN_PROP_DEFAULT_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_2
#define YB_GEN_PROP_DEFAULT_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_2
#define YB_GEN_PROP_DEFAULT_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_2
#define YB_GEN_PROP_DEFAULT_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_PROP_DEFAULT_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_1
#define YB_GEN_PROP_DEFAULT_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_1
#define YB_GEN_PROP_DEFAULT_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_DO_YB_GEN_COL_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_PROP_DEFAULT_1
#define YB_GEN_PROP_DEFAULT_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_PROP_DEFAULT(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_PROP_DEFAULT_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_41_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_42
#define YB_GEN_CREATE_RELS_41_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(41, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_42
#define YB_GEN_CREATE_RELS_41_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_42
#define YB_GEN_CREATE_RELS_41_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_41(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_41_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_40_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_41
#define YB_GEN_CREATE_RELS_40_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(40, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_41
#define YB_GEN_CREATE_RELS_40_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_41
#define YB_GEN_CREATE_RELS_40_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_40(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_40_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_39_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_40
#define YB_GEN_CREATE_RELS_39_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(39, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_40
#define YB_GEN_CREATE_RELS_39_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_40
#define YB_GEN_CREATE_RELS_39_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_39(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_39_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_38_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_39
#define YB_GEN_CREATE_RELS_38_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(38, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_39
#define YB_GEN_CREATE_RELS_38_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_39
#define YB_GEN_CREATE_RELS_38_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_38(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_38_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_37_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_38
#define YB_GEN_CREATE_RELS_37_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(37, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_38
#define YB_GEN_CREATE_RELS_37_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_38
#define YB_GEN_CREATE_RELS_37_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_37(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_37_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_36_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_37
#define YB_GEN_CREATE_RELS_36_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(36, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_37
#define YB_GEN_CREATE_RELS_36_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_37
#define YB_GEN_CREATE_RELS_36_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_36(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_36_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_35_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_36
#define YB_GEN_CREATE_RELS_35_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(35, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_36
#define YB_GEN_CREATE_RELS_35_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_36
#define YB_GEN_CREATE_RELS_35_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_35(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_35_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_34_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_35
#define YB_GEN_CREATE_RELS_34_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(34, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_35
#define YB_GEN_CREATE_RELS_34_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_35
#define YB_GEN_CREATE_RELS_34_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_34(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_34_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_33_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_34
#define YB_GEN_CREATE_RELS_33_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(33, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_34
#define YB_GEN_CREATE_RELS_33_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_34
#define YB_GEN_CREATE_RELS_33_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_33(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_33_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_32_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_33
#define YB_GEN_CREATE_RELS_32_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(32, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_33
#define YB_GEN_CREATE_RELS_32_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_33
#define YB_GEN_CREATE_RELS_32_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_32(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_32_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_31_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_32
#define YB_GEN_CREATE_RELS_31_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(31, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_32
#define YB_GEN_CREATE_RELS_31_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_32
#define YB_GEN_CREATE_RELS_31_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_31(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_31_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_30_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_31
#define YB_GEN_CREATE_RELS_30_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(30, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_31
#define YB_GEN_CREATE_RELS_30_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_31
#define YB_GEN_CREATE_RELS_30_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_30(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_30_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_29_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_30
#define YB_GEN_CREATE_RELS_29_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(29, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_30
#define YB_GEN_CREATE_RELS_29_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_30
#define YB_GEN_CREATE_RELS_29_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_29(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_29_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_28_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_29
#define YB_GEN_CREATE_RELS_28_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(28, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_29
#define YB_GEN_CREATE_RELS_28_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_29
#define YB_GEN_CREATE_RELS_28_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_28(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_28_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_27_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_28
#define YB_GEN_CREATE_RELS_27_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(27, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_28
#define YB_GEN_CREATE_RELS_27_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_28
#define YB_GEN_CREATE_RELS_27_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_27(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_27_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_26_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_27
#define YB_GEN_CREATE_RELS_26_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(26, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_27
#define YB_GEN_CREATE_RELS_26_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_27
#define YB_GEN_CREATE_RELS_26_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_26(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_26_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_25_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_26
#define YB_GEN_CREATE_RELS_25_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(25, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_26
#define YB_GEN_CREATE_RELS_25_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_26
#define YB_GEN_CREATE_RELS_25_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_25(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_25_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_24_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_25
#define YB_GEN_CREATE_RELS_24_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(24, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_25
#define YB_GEN_CREATE_RELS_24_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_25
#define YB_GEN_CREATE_RELS_24_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_24(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_24_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_23_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_24
#define YB_GEN_CREATE_RELS_23_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(23, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_24
#define YB_GEN_CREATE_RELS_23_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_24
#define YB_GEN_CREATE_RELS_23_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_23(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_23_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_22_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_23
#define YB_GEN_CREATE_RELS_22_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(22, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_23
#define YB_GEN_CREATE_RELS_22_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_23
#define YB_GEN_CREATE_RELS_22_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_22(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_22_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_21_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_22
#define YB_GEN_CREATE_RELS_21_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(21, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_22
#define YB_GEN_CREATE_RELS_21_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_22
#define YB_GEN_CREATE_RELS_21_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_21(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_21_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_20_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_21
#define YB_GEN_CREATE_RELS_20_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(20, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_21
#define YB_GEN_CREATE_RELS_20_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_21
#define YB_GEN_CREATE_RELS_20_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_20(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_20_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_19_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_20
#define YB_GEN_CREATE_RELS_19_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(19, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_20
#define YB_GEN_CREATE_RELS_19_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_20
#define YB_GEN_CREATE_RELS_19_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_19(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_19_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_18_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_19
#define YB_GEN_CREATE_RELS_18_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(18, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_19
#define YB_GEN_CREATE_RELS_18_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_19
#define YB_GEN_CREATE_RELS_18_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_18(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_18_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_17_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_18
#define YB_GEN_CREATE_RELS_17_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(17, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_18
#define YB_GEN_CREATE_RELS_17_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_18
#define YB_GEN_CREATE_RELS_17_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_17(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_17_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_16_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_17
#define YB_GEN_CREATE_RELS_16_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(16, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_17
#define YB_GEN_CREATE_RELS_16_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_17
#define YB_GEN_CREATE_RELS_16_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_16(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_16_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_15_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_16
#define YB_GEN_CREATE_RELS_15_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(15, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_16
#define YB_GEN_CREATE_RELS_15_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_16
#define YB_GEN_CREATE_RELS_15_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_15(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_15_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_14_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_15
#define YB_GEN_CREATE_RELS_14_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(14, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_15
#define YB_GEN_CREATE_RELS_14_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_15
#define YB_GEN_CREATE_RELS_14_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_14(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_14_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_13_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_14
#define YB_GEN_CREATE_RELS_13_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(13, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_14
#define YB_GEN_CREATE_RELS_13_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_14
#define YB_GEN_CREATE_RELS_13_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_13(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_13_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_12_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_13
#define YB_GEN_CREATE_RELS_12_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(12, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_13
#define YB_GEN_CREATE_RELS_12_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_13
#define YB_GEN_CREATE_RELS_12_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_12(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_12_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_11_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_12
#define YB_GEN_CREATE_RELS_11_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(11, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_12
#define YB_GEN_CREATE_RELS_11_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_12
#define YB_GEN_CREATE_RELS_11_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_11(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_11_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_10_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_11
#define YB_GEN_CREATE_RELS_10_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(10, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_11
#define YB_GEN_CREATE_RELS_10_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_11
#define YB_GEN_CREATE_RELS_10_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_10(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_10_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_9_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_10
#define YB_GEN_CREATE_RELS_9_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(9, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_10
#define YB_GEN_CREATE_RELS_9_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_10
#define YB_GEN_CREATE_RELS_9_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_9(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_9_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_8_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_9
#define YB_GEN_CREATE_RELS_8_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(8, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_9
#define YB_GEN_CREATE_RELS_8_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_9
#define YB_GEN_CREATE_RELS_8_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_8(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_8_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_7_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_8
#define YB_GEN_CREATE_RELS_7_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(7, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_8
#define YB_GEN_CREATE_RELS_7_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_8
#define YB_GEN_CREATE_RELS_7_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_7(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_7_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_6_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_7
#define YB_GEN_CREATE_RELS_6_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(6, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_7
#define YB_GEN_CREATE_RELS_6_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_7
#define YB_GEN_CREATE_RELS_6_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_6(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_6_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_5_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_6
#define YB_GEN_CREATE_RELS_5_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(5, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_6
#define YB_GEN_CREATE_RELS_5_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_6
#define YB_GEN_CREATE_RELS_5_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_5(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_5_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_4_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_5
#define YB_GEN_CREATE_RELS_4_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(4, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_5
#define YB_GEN_CREATE_RELS_4_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_5
#define YB_GEN_CREATE_RELS_4_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_4(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_4_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_3_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_4
#define YB_GEN_CREATE_RELS_3_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(3, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_4
#define YB_GEN_CREATE_RELS_3_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_4
#define YB_GEN_CREATE_RELS_3_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_3(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_3_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_2_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_3
#define YB_GEN_CREATE_RELS_2_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(2, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_3
#define YB_GEN_CREATE_RELS_2_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_3
#define YB_GEN_CREATE_RELS_2_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_2(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_2_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_1_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_2
#define YB_GEN_CREATE_RELS_1_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(1, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_2
#define YB_GEN_CREATE_RELS_1_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_2
#define YB_GEN_CREATE_RELS_1_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS_1(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_1_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#define YB_GEN_CREATE_RELS_0_YB_GEN_REL_MANY_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_MANY_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_1
#define YB_GEN_CREATE_RELS_0_YB_GEN_REL_ONE_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_DO_YB_GEN_REL_ONE_(0, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) YB_GEN_CREATE_RELS_1
#define YB_GEN_CREATE_RELS_0_YB_GEN_COL_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_1
#define YB_GEN_CREATE_RELS_0_YB_GEN_COL_END_(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)
#define YB_GEN_CREATE_RELS(kind, p1, p2, p3, p4, p5, p6, p7, p8, p9, p10) \
    YB_GEN_CREATE_RELS_0_##kind(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10)

#endif
