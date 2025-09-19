#pragma once

class IndexBuffer {
    private:
        unsigned int m_RendererID;
        unsigned int m_Count = 0;
    public:
    // sizeof *data could be a larger or smaller int for memory optimization purposes
        IndexBuffer();
        IndexBuffer(const unsigned int* data, unsigned int count);
        ~IndexBuffer();

        void Bind() const;
        void Unbind() const;
        void Update(const void* data, unsigned int size, unsigned int offset = 0);
        void setData(const unsigned int* data, unsigned int count);

        inline unsigned int getCount() const {
            return m_Count;
        }
};
