#pragma once

#include <vector>


class MeshData
{
    public:
        MeshData();
        ~MeshData();

    private:
        std::vector<float> vertices;
};