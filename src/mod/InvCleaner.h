#pragma once

#include <ll/api/plugin/NativePlugin.h>
#include <ll/api/io/Logger.h>
#include "mc/nbt/ListTag.h"

namespace inv_cleaner {

class InvCleanerMod {
public:
    static InvCleanerMod& getInstance();

    explicit InvCleanerMod(ll::plugin::NativePlugin& self)
    : mSelf(self),
      mLogger(self.getSelf().getLogger()) {}

    InvCleanerMod(const InvCleanerMod&) = delete;
    InvCleanerMod(InvCleanerMod&&)      = delete;

    [[nodiscard]] ll::plugin::NativePlugin& getSelf() const { return mSelf; }

    bool load();
    bool enable();
    bool disable();

private:
    void registerCommand();
    void doClearAllPlayers(const std::string& targetItemName, short targetItemAux);
    int  clearItemsInList(ListTag& itemList, const std::string& targetItemName, short targetItemAux);

    ll::plugin::NativePlugin& mSelf;
    ll::io::Logger&           mLogger;
};

} // namespace inv_cleaner
