/*
 *  File:       invent.cc
 *  Summary:    Functions for inventory related commands.
 *  Written by: Linley Henzell
 *
 *  Modified for Crawl Reference by $Author$ on $Date$
 *
 *  Change History (most recent first):
 *
 *               <1>     -/--/--        LRH             Created
 */


#ifndef INVENT_H
#define INVENT_H

#include <stddef.h>
#include <vector>
#include "menu.h"
#include "enum.h"

#define PROMPT_ABORT        -1
#define PROMPT_GOT_SPECIAL  -2

struct SelItem
{
    int slot;
    int quantity;
    const item_def *item;

    SelItem() : slot(0), quantity(0), item(NULL) { }
    SelItem( int s, int q, const item_def *it = NULL )
        : slot(s), quantity(q), item(it) 
    {
    }
};

struct InvTitle : public MenuEntry
{
    Menu *m;
    std::string (*titlefn)( int menuflags, const std::string &oldt );
    
    InvTitle( Menu *mn, const std::string &title,
              std::string (*tfn)( int menuflags, const std::string &oldt ) );

    std::string get_text() const;
};

class InvShowPrices;
class InvEntry : public MenuEntry
{
private:
    static bool show_prices;
    static char temp_id[4][50];
    static void set_show_prices(bool doshow);

    friend class InvShowPrices;

public:
    const item_def *item;

    InvEntry( const item_def &i );
    std::string get_text() const;

private:
    void add_class_hotkeys(const item_def &i);
};

class InvShowPrices {
public:
    InvShowPrices(bool doshow = true);
    ~InvShowPrices();
};

class InvMenu : public Menu
{
public:
    InvMenu(int mflags = MF_MULTISELECT) 
        : Menu(mflags), type(MT_INVSELECT), title_annotate(NULL)
    {
    }

    unsigned char getkey() const;

    void set_type(menu_type t);

    // Sets function to annotate the title with meta-information if needed.
    // If you set this, do so *before* calling set_title, or it won't take
    // effect.
    void set_title_annotator(std::string (*fn)(int, const std::string &));

    void set_title(MenuEntry *title);
    void set_title(const std::string &s);

    // Loads items into the menu. If "procfn" is provided, it'll be called
    // for each MenuEntry added.
    // NOTE: Does not set menu title, ever! You *must* set the title explicitly
    void load_items(const std::vector<const item_def*> &items,
                    void (*procfn)(MenuEntry *me) = NULL);

    // Loads items from the player's inventory into the menu, and sets the
    // title to the stock title. If "procfn" is provided, it'll be called for
    // each MenuEntry added, *excluding the title*.
    void load_inv_items(int item_selector = OSEL_ANY,
                        void (*procfn)(MenuEntry *me) = NULL);

    std::vector<SelItem> get_selitems() const;

    // Returns vector of item_def pointers to each item_def in the given
    // vector. Note: make sure the original vector stays around for the lifetime
    // of the use of the item pointers, or mayhem results!
    static std::vector<const item_def*> xlat_itemvect(
            const std::vector<item_def> &);
protected:
    bool process_key(int key);

protected:
    menu_type type;
    std::string (*title_annotate)(int mflags, const std::string &oldtitle);
};


int prompt_invent_item( const char *prompt,
                        menu_type type,
                        int type_expect,
                        bool must_exist = true, 
                        bool allow_auto_list = true, 
                        bool allow_easy_quit = true,
                        const char other_valid_char = '\0',
                        int *const count = NULL,
                        operation_types oper = OPER_ANY );

std::vector<SelItem> select_items(
                        const std::vector<const item_def*> &items, 
                        const char *title, bool noselect = false );

std::vector<SelItem> prompt_invent_items(
                        const char *prompt,
                        menu_type type,
                        int type_expect,
                        std::string (*titlefn)( int menuflags, 
                                                const std::string &oldt ) 
                            = NULL,
                        bool allow_auto_list = true,
                        bool allow_easy_quit = true,
                        const char other_valid_char = '\0',
                        std::vector<text_pattern> *filter = NULL,
                        Menu::selitem_tfn fn = NULL,
                        const std::vector<SelItem> *pre_select = NULL );


// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: invent - ouch - shopping
 * *********************************************************************** */
unsigned char invent( int item_class_inv, bool show_price );

unsigned char invent_select(
                   menu_type type,
                   int item_class_inv,
                   int select_flags = MF_NOSELECT,
                   std::string (*titlefn)( int menuflags, 
                                           const std::string &oldt ) 
                        = NULL,
                   std::vector<SelItem> *sels = NULL,
                   std::vector<text_pattern> *filter = NULL,
                   Menu::selitem_tfn fn = NULL,
                   const std::vector<SelItem> *pre_select = NULL );

// last updated 24may2000 {dlb}
/* ***********************************************************************
 * called from: acr - command - food - item_use - items - spl-book - spells1
 * *********************************************************************** */
unsigned char get_invent(int invent_type);

bool in_inventory(const item_def &i);

// last updated 12may2000 {dlb}
/* ***********************************************************************
 * called from: acr
 * *********************************************************************** */
void list_commands(bool wizard);

std::string item_class_name(int type, bool terse = false);

#endif
