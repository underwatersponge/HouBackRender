#pragma once
#include <imgui.h>
#include <corecrt_search.h>
#include <string>
#include <vector>
#include <iostream>

struct DualListBox
{
public:
    DualListBox(std::vector<std::string>& data)
        :CustomUserData(data)
    {
        FullItems();
    }
    
    ImVector<ImGuiID>           Items[2];    
    ImGuiSelectionBasicStorage  Selections[2];
    bool                        OptKeepSorted = true;
    std::vector<std::string>&   CustomUserData;// should use basicstorage userdata?
    
    void ResetCustomUserData(std::vector<std::string>& data)
    {
        CustomUserData = data;
        for (int i = 0; i < 2; i++)
        {
            Selections[i].Clear();
        }
        for (int i=0; i<2; i++)
        {
            Items[i].clear();
        }
        FullItems();
    }
    
    void FullItems()
    {
        if (Items[0].Size == 0 && Items[1].Size == 0)
        {
            for (int itemId=0; itemId<CustomUserData.size(); itemId++)
            {
                Items[0].push_back((ImGuiID)itemId);
            }
        }
    }
    
    void MoveAll(int src, int dst)
    {
        if ((src == 0 && dst == 1) || (src == 1 && dst == 0))
        {
            for (ImGuiID item_id : Items[src])
                Items[dst].push_back(item_id);
            Items[src].clear();
            SortItems(dst);
            Selections[src].Swap(Selections[dst]);
            Selections[src].Clear();
        }
    }
    
    void MoveSelected(int src, int dst)
    {
        for (int src_n = 0; src_n < Items[src].size(); src_n++)
        {
            ImGuiID item_id = Items[src][src_n];
            if (!Selections[src].Contains(item_id))
                continue;
            Items[src].erase(&Items[src][src_n]);
            Items[dst].push_back(item_id);
            src_n--;
        }
        if (OptKeepSorted)
            SortItems(dst);
        Selections[src].Swap(Selections[dst]);
        Selections[src].Clear();
    }
    
    void ApplySelectionRequests(ImGuiMultiSelectIO* ms_io, int side)
    {
        Selections[side].UserData = Items[side].Data;
        Selections[side].AdapterIndexToStorageId = [](ImGuiSelectionBasicStorage* self, int idx){ImGuiID* items = (ImGuiID*)self->UserData; return items[idx];};
        Selections[side].ApplyRequests(ms_io);
    }
    
    static int __cdecl CompareItemsByValue(const void* lhs, const void* rhs)
    {
        const int* a = (const int*)lhs;
        const int* b = (const int*)rhs;
        return *a - *b;
    }
    
    void SortItems(int n)
    {
        qsort(Items[n].Data, (size_t)Items[n].Size, sizeof(Items[n][0]), CompareItemsByValue);
    }
    
    void Show()
    {
        if (ImGui::BeginTable("split", 3, ImGuiTableFlags_None))
        {
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);    // Left side
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed);      // Buttons
            ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthStretch);    // Right side
            ImGui::TableNextRow();
            
            int request_move_selected = -1;
            int request_move_all = -1;
            float child_height_0 = 0.f;
            for (int side=0; side<2; side++)
            {
                ImVector<ImGuiID>& items = Items[side];
                ImGuiSelectionBasicStorage& selection = Selections[side];
                
                ImGui::TableSetColumnIndex((side == 0) ? 0 : 2 );
                ImGui::Text("%s (%d)", (side==0)? "From" : "To", items.Size);
                
                const float items_height = ImGui::GetTextLineHeightWithSpacing();
                ImGui::SetNextWindowContentSize(ImVec2(0.0f, items.Size * items_height));
                
                bool child_visible;
                if (side == 0)
                {
                    // left child is resizable
                    ImGui::SetNextWindowSizeConstraints(ImVec2(0.0f, ImGui::GetFrameHeightWithSpacing() * 4), ImVec2(FLT_MAX, FLT_MAX));
                    child_visible = ImGui::BeginChild("0", ImVec2(-FLT_MIN, ImGui::GetFontSize() * 20), ImGuiChildFlags_FrameStyle | ImGuiChildFlags_ResizeY);
                    child_height_0 = ImGui::GetWindowSize().y;
                }
                else
                {
                    // Right child use same height as left one
                    child_visible = ImGui::BeginChild("1", ImVec2(-FLT_MIN, child_height_0), ImGuiChildFlags_FrameStyle);
                }
                if (child_visible)
                {
                    ImGuiMultiSelectFlags flags = ImGuiMultiSelectFlags_BoxSelect1d;
                    ImGuiMultiSelectIO* ms_io = ImGui::BeginMultiSelect(flags, selection.Size, items.Size);
                    ApplySelectionRequests(ms_io, side);
                    
                    for (int item_n=0; item_n<items.Size; item_n++)
                    {
                        ImGuiID item_id = items[item_n];
                        bool item_is_selected = selection.Contains(item_id);
                        ImGui::SetNextItemSelectionUserData(item_n);
                        ImGui::Selectable(CustomUserData[item_id].c_str(), item_is_selected, ImGuiSelectableFlags_AllowDoubleClick);
                        if (ImGui::IsItemFocused)
                        {
                            if (ImGui::IsKeyPressed(ImGuiKey_Enter) || ImGui::IsKeyPressed(ImGuiKey_KeypadEnter))
                                request_move_selected = side;
                            if (ImGui::IsMouseDoubleClicked(0))
                                request_move_selected = side;
                        }
                    }
                    ms_io = ImGui::EndMultiSelect();
                    ApplySelectionRequests(ms_io, side);
                }
                ImGui::EndChild();
            }
            
            // Button colums
            ImGui::TableSetColumnIndex(1);
            ImGui::NewLine();
            ImVec2 button_sz = {ImGui::GetFrameHeight(), ImGui::GetFrameHeight()};
            
            if (ImGui::Button(">>", button_sz))
                request_move_all = 0;
            if (ImGui::Button(">", button_sz))
                request_move_selected = 0;
            if (ImGui::Button("<", button_sz))
                request_move_selected = 1;
            if (ImGui::Button("<<", button_sz))
                request_move_all = 1;

            // Process requests
            if (request_move_all != -1)
                MoveAll(request_move_all, request_move_all ^ 1);
            if (request_move_selected != -1)
                MoveSelected(request_move_selected, request_move_selected ^ 1);
            
            ImGui::EndTable();
        }
    }
};





