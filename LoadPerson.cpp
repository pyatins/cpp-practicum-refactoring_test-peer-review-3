struct DBInfo {
    std::string_view name;
    int connection_timeout = 0;
    bool allow_exceptions = false;
    DBLogLevel log_level;
};

struct FilterInfo {
    int min_age = 0;
    int max_age = 0;
    std::string_view name_filter;
};

std::optional<DBHandler> DBConnect(const DBInfo& db_info) {
    DBHandler result;
    DBConnector connector(db_info.allow_exceptions, db_info.log_level);
    if (db_info.name.starts_with("tmp."s)) {
        result = connector.ConnectTmp(db_info.name, db_info.connection_timeout);
    } else {
        result = connector.Connect(db_info.name, db_info.connection_timeout);
    }
    if (!result.IsOK() && !db_info.allow_exceptions) {
        return std::nullopt;
    }
    return result;
}

DBQuery GetQuery(const DBHandler& db, const FilterInfo& filter) {
    std::ostringstream query_str;
    query_str << "from Persons select Name, Age where Age between "s
              << filter.min_age << " and "s << filter.max_age
              << " and Name like '%"s << db.Quote(filter.name_filter) << "%'"s;
    return DBQuery{query_str.str()};
}

std::vector<Person> LoadPersons(const DBInfo& db_info, const FilterInfo& filter) {
    std::optional<DBHandler> db = DBConnect(db_info);
    if (!db) {
        return {};
    }
    DBQuery query = GetQuery(*db, filter);
    std::vector<Person> persons;
    for (auto [name, age] : db->LoadRows<std::string, int>(query)) {
        persons.emplace_back(std::move(name), age);
    }
    return persons;
}