#pragma once
#include "ofMain.h"


class Location {
public:
    std::string id;
    std::string name;
    std::string coords;
    std::string scenario;
    std::string future_date;
    std::string temperature;
    std::string humidity;
    std::string rainfall;
    Location(const std::string& id, const std::string& name, const std::string& coords, const std::string& scenario, const std::string& future_date, const std::string& temperature, const std::string& humidity, const std::string& rainfall) :
        id(id),
        name(name),
        coords(coords),
        scenario(scenario),
        future_date(future_date),
        temperature(temperature),
        humidity(humidity),
        rainfall(rainfall)
    { }

    Location(const Location & x) :
        id(x.id),
        name(x.name),
        coords(x.coords),
        scenario(x.scenario),
        future_date(x.future_date),
        temperature(x.temperature),
        humidity(x.humidity),
        rainfall(x.rainfall)
    { }

    static const std::vector<Location> locations;

};