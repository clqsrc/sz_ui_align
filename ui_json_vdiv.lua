--[[ Generated with https://github.com/TypeScriptToLua/TypeScriptToLua ]]
function MakeVDiv(view, uijson)
    local this_
    local t = {
        uijson = uijson,
        view = view,
        class = "div",
        AsEdit = function()
            this_.class = "edit"
            return this_
        end,
        AsImage = function()
            this_.class = "image"
            return this_
        end,
        Show = function()
            View_Show(view)
        end,
        Hide = function()
            View_Hide(view)
        end,
        SetOnClick = function(obj, func)
            local view_handle = view
            uijson:SetOnClick(view_handle, obj, func)
        end,
        SetCaption = function(txt)
            UI_Div_SetTxt_forObj(uijson.uijson_handle, view, txt)
        end,
        SetText = function(txt)
            if "edit" == this_.class then
                View_Edit_SetText(view, txt)
            else
                UI_Div_SetTxt_forObj(uijson.uijson_handle, view, txt)
            end
        end,
        SetEditText = function(txt)
            View_Edit_SetText(view, txt)
        end,
        GetEditText = function()
            return View_Edit_GetText(view)
        end,
        SetRect = function(rect)
            View_SetRect(view, rect)
        end,
        GetRect = function()
            return View_GetRect(view)
        end,
        SetImage = function(fn)
            UI_Div_SetImage_forObj(uijson.uijson_handle, view, fn)
        end,
        SetBackgroundColor = function(hex_color)
            View_SetBackgroundColor(view, hex_color)
        end,
        SetRadius = function(radius)
            UI_Div_SetRadius_forObj(uijson.uijson_handle, view, radius)
        end
    }
    this_ = t
    return t
end
