#pragma once

class VertexBuffer {

    private:

        unsigned int m_RendererID;

    public:

        VertexBuffer();
        VertexBuffer(const void* data, unsigned int size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;
        void Update(const void* data, unsigned int size, unsigned int offset = 0);
        void setData(const void* data, unsigned int size);

        inline unsigned int getID() const {
            return m_RendererID;
        }

};
