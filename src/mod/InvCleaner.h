#pragma once

#include <ll/api/mod/NativeMod.h>
#include <ll/api/io/Logger.h>
#include "mc/nbt/ListTag.h"

namespace inv_cleaner {

class InvCleanerMod {
public:
    static InvCleanerMod& getInstance();

    explicit InvCleanerMod(ll::mod::NativeMod& self)
    : mSelf(self),
      mLogger(self.getSelf().getLogger()) {}

    InvCleanerMod(const InvCleanerMod&) = delete;
    InvCleanerMod(InvCleanerMod&&)      = delete;

    [[nodiscard]] ll::mod::NativeMod& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();

private:
    void registerCommand();
    void doClearAllPlayers(const std::string& targetItemName, short targetItemAux);
    int  clearItemsInList(ListTag& itemList, const std::string& targetItemName, short targetItemAux);

    ll::mod::NativeMod& mSelf; // 类型更新为 NativeMod
    ll::io::Logger&     mLogger;
};

} // namespace inv_cleaner
