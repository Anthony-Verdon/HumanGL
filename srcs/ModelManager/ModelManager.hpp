#pragma once

#include <vector>
#include "Model/Model.hpp"

class ModelManager
{
    public:
        static Model &GetModel(int index) { return (models[index]); }
        static void AddModels(const std::vector<Model> &newModels) { models.insert(ModelManager::models.end(), newModels.begin(), newModels.end()); }
        static size_t GetNbModel() { return (models.size()); }

    private:
        ModelManager();
        ~ModelManager();
            
        static std::vector<Model> models;
};