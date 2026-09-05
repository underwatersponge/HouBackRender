#pragma once
#include <imgui.h>
#include <ostream>
#include <string>
#include <vector>

// should i just add imgui_demo.cpp?
#define IM_MIN(A, B)            (((A) < (B)) ? (A) : (B))
struct SelectionWithDeletion : public ImGuiSelectionBasicStorage
{
    int ApplyDeletionPreLoop(ImGuiMultiSelectIO* ms_io, int items_count)
    {
        if (Size == 0)
            return -1;
        
        // If focused item is not selected...
        const int focused_idx = (int)ms_io->NavIdItem;
        if (ms_io->NavIdSelected == false)
        {
            ms_io->RangeSrcReset = true;
            return focused_idx;
        }
        // If focused item is selected:land on first unselected item after focused item.
        for (int idx = focused_idx; idx < items_count; idx++)
        {
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;
        }
        
        // If focused item is selected: otherwise return last unselected item before focused item
        for (int idx = IM_MIN(focused_idx, items_count) - 1; idx >=0; idx--)
            if (!Contains(GetStorageIdFromIndex(idx)))
                return idx;
        
        return -1;
    }
    
    template<typename ITEM_TYPE>
    void ApplyDeletionPostLoop(ImGuiMultiSelectIO* ms_io, std::vector<ITEM_TYPE>& items, int item_curr_idx_to_select)
    {
        int keepNum = items.size() - Size;
        int item_next_idx_to_select = -1;
       
        // TODO:use a better way(should be can use i)
        std::vector<std::string> temp;
        for (int i=0; i<items.size(); i++)
        {
            if (!Contains(GetStorageIdFromIndex(i)))
            {
                temp.push_back(items.at(i));
            }
            if (item_curr_idx_to_select == i)
            {
                item_next_idx_to_select = keepNum - 1;
            }
        }
        items.swap(temp);
        Clear();
        if (item_next_idx_to_select != -1 && ms_io->NavIdSelected)
            SetItemSelected(GetStorageIdFromIndex(item_next_idx_to_select), true);
    }
    int toClean = -1;
};
