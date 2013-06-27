class Chart {
    public:
        virtual void draw() = 0;
     
        float getWidth() { return width; }
        void setWidth(float w) { width = w; }

        float getHeight() { return height; }
        void setHeight(float h) { height = h; }
    protected:
        float width, height;
        float round(float num);
        float roundDown(float num);
        float roundUp(float num);
    private:        
        float f(float num, float c);
        float g(float num, float c);
};