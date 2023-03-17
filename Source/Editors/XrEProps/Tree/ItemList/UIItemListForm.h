#pragma once
class XREPROPS_API UIItemListForm: public XrUI
{
    friend class UIItemListFormItem;
    TOnILItemsFocused OnItemsFocusedEvent;
    TOnILItemFocused  OnItemFocusedEvent;
    TOnItemRemove     OnItemRemoveEvent;
    TOnItemRename     OnItemRenameEvent;
    TOnItemCreate     OnItemCreateEvent;
    TOnItemClone      OnItemCloneEvent;

public:
    UIItemListForm();
    virtual ~UIItemListForm();

public:
    virtual void Draw();
    void         ClearList();
    void         ClearSelected();
    void         SelectItem(const char* name);
    void         AssignItems(
                ListItemsVec& items,
                const char*   name_selection = nullptr,
                bool          clear_floder   = true,
                bool          save_selected  = false);
    IC const ListItemsVec& GetItems() const
    {
        return m_Items;
    }
    bool GetSelected(RStringVec& items) const;
    int  GetSelected(LPCSTR pref, ListItemsVec& items, bool bOnlyObject);

public:
    IC void SetOnItemsFocusedEvent(TOnILItemsFocused e)
    {
        OnItemsFocusedEvent = e;
    }
    IC void SetOnItemFocusedEvent(TOnILItemFocused e)
    {
        OnItemFocusedEvent = e;
    }
    IC void SetOnItemRemoveEvent(TOnItemRemove e)
    {
        OnItemRemoveEvent = e;
    }
    IC void SetOnItemRenameEvent(TOnItemRename e)
    {
        OnItemRenameEvent = e;
    }
    IC void SetOnItemCreaetEvent(TOnItemCreate e)
    {
        OnItemCreateEvent = e;
    }
    IC void SetOnItemCloneEvent(TOnItemClone e)
    {
        OnItemCloneEvent = e;
    }

public:
    enum
    {
        fMenuEdit    = (1 << 0),
        fMultiSelect = (1 << 1),
    };
    Flags32      m_Flags;
    ListItemsVec m_Items;

private:
    void                UpdateSelected(UIItemListFormItem* NewSelected);
    UIItemListFormItem  m_RootItem;
    UIItemListFormItem* m_SelectedItem;
};
