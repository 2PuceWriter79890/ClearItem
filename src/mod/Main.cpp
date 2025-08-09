#include "mod/InvCleaner.h"

#include <ll/api/mod/RegisterHelper.h>
LL_MOD_ENTRY_HOOK(
    inv_cleaner_main,
    ll::mod::NativeMod&,
    ll::mod::ModLoadReason reason
) {
    if (reason == ll::mod::ModLoadReason::Load) {
        inv_cleaner::InvCleanerMod::getInstance().load();
    }
    return true;
}

LL_MOD_ENABLE_HOOK(inv_cleaner_main, ll::mod::NativeMod&) {
    inv_cleaner::InvCleanerMod::getInstance().enable();
    return true;
}

LL_MOD_DISABLE_HOOK(inv_cleaner_main, ll::mod::NativeMod&) {
    inv_cleaner::InvCleanerMod::getInstance().disable();
    return true;
}

LL_REGISTER_MOD(
    "InvCleaner",
    "Asynchronously cleans items from all players.",
    "1.0.0",
    {{"author", "Puce"}}
);
