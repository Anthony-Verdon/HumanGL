#pragma once

#include "Mesh/MeshData/MeshData.hpp"

class MeshRenderer: public MeshData
{
    public:
        MeshRenderer(const MeshData &data);
        MeshRenderer(const MeshRenderer &instance);
        MeshRenderer &operator=(const MeshRenderer &instance);
        ~MeshRenderer();

        void InitRenderer();
        void DestroyRenderer();
        
        void Draw(const AlgOps::mat4 &rotation, const AlgOps::mat4 &projection, const AlgOps::mat4 &view, const AlgOps::mat4 &model) const;

        void SetParentRenderer(MeshRenderer *parent) { parentRenderer = parent; }
        MeshRenderer *GetParentRenderer() { return (parentRenderer); }
        const MeshRenderer *GetRoot() const;
        const MeshRenderer *GetNode(int ID) const;
    private:
        unsigned int VAO;
        unsigned int VBO;
        unsigned int EBO;

        std::vector<MeshRenderer> childrenRenderer;
        MeshRenderer *parentRenderer;

};