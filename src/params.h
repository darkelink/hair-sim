#pragma once

#include <map>
#include <string>
#include <vector>

class Params {
    public:
        bool Add(std::string param, float value);
        bool Exists(std::string);
        bool Modify(std::string param, float value);
        float Get_value(std::string param);

        std::vector<std::string> Get_params();
    private:
        std::map<std::string, float> params;
};
