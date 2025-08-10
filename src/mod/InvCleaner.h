#pragma once

#include <ll/api/mod/NativeMod.h>
#include <ll/api/io/Logger.h>
#include <mc/nbt/ListTag.h>

namespace inv_cleaner {

class InvCleanerMod {
public:
    InvCleanerMod()
    : mSelf(*ll::mod::NativeMod::current()),
      mLogger(mSelf.getLogger()) {}

    bool load();
    bool enable();
    bool disable();

private:
    void registerCommand();
    void doClearAllPlayers(const std::string& targetItemName, short targetItemAux);
    int  clearItemsInList(ListTag& itemList, const std::string& targetItemName, short targetItemAux);

    ll::mod::NativeMod& mSelf;
    ll::io::Logger&     mLogger;
};

} // namespace inv_cleaner
