#ifndef menuitem_h
#define menuitem_h

class button{
private:
    int posX;
    int posY;
    int width;
    int height;
public:
    button(int x,int y,int width,int height);
    bool checkArea(int x,int y);
};

#endif // menuitem_h
