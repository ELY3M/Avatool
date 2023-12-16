#include <string>
#include <switch.h>

#include "graphics.hpp"
#include "menu.hpp"

void menu::setParams(const unsigned& _x, const unsigned& _y, const unsigned& _rW)
{
    x = _x;
    y = _y;
    rW = _rW;
    rY = _y;
}

void menu::addOpt(const std::string& add)
{
    opt.push_back(add);
}

void menu::editOpt(int ind, const std::string& ch)
{
    opt[ind] = ch;
}

menu::~menu()
{
    opt.clear();
}

bool menu::handleInput(const uint64_t& down)
{
    int size = opt.size() - 1;
    if(down & HidNpadButton_Up)
    {
        selected--;
        if(selected < 0)
            selected = size;

        if((start > selected)  && (start > 0))
            start--;
        if(size < 11)
            start = 0;
        if((selected - 10) > start)
            start = selected - 10;

        return true;
    }
    else if(down & HidNpadButton_Down)
    {
        selected++;
        if(selected > size)
            selected = 0;

        if((selected > (start + 9)) && ((start + 9) < size))
            start++;
        if(selected == 0)
            start = 0;

        return true;
    }
    else if(down & HidNpadButton_Right)
    {
        selected += 5;
        if(selected > size)
            selected = size;
        if((selected - 10) > start)
            start = selected - 10;

        return true;
    }
    else if(down & HidNpadButton_Left)
    {
        selected -= 5;
        if(selected < 0)
            selected = 0;
        if(selected < start)
            start = selected;

        return true;
    }

    return false;
}

void menu::draw(graphics *gfx, const uint32_t& color)
{
    if(opt.size() == 0)
        return;

    if(clrAdd)
    {
        clrSh += 4;
        if(clrSh > 63)
            clrAdd = false;
    }
    else
    {
        clrSh--;
        if(clrSh == 0)
            clrAdd = true;
    }

    int length = 0;
    if((opt.size() - 1) < 10)
        length = opt.size();
    else
        length = start + 11;

    uint32_t selectionColor = createColor(0x00, (0x60 + clrSh), (0xBB + clrSh), 0xFF);
    for(int i = start; i < length; i++)
    {
        if(i == selected)
        {
            gfx->renderRectFill(NULL, selectionColor, x, y + ((i - start) * 36), rW, 32);
        }

        gfx->renderTextf(NULL, 18, COLOR_WHITE, x + 8, (y + 8) + ((i - start) * 36), opt[i].c_str());
    }
}

void menu::reset()
{
    opt.clear();

    fc = 0;
}

void menu::adjust()
{
    if(selected > (int)opt.size() - 1)
        selected = opt.size() - 1;

    if(opt.size() < 10)
        start = 0;
    else if(opt.size() > 10 && start + 10 > (int)opt.size() - 1)
        start--;
}
