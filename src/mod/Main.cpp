#include "mod/InvCleaner.h"
#include <ll/api/mod/RegisterHelper.h>

namespace {

inv_cleaner::InvCleanerMod instance;

} // namespace

LL_MOD_ENTRY_HOOK(
    inv_cleaner_main,
    ll::mod::NativeMod&,
    ll::mod::ModLoadReason reason
) {
    if (reason == ll::mod::ModLoadReason::Load) {
        instance.load();
    }
    return true;
}

LL_MOD_ENABLE_HOOK(inv_cleaner_main, ll::mod::NativeMod&) {
    return instance.enable();
}

LL_MOD_DISABLE_HOOK(inv_cleaner_main, ll::mod::NativeMod&) {
    return instance.disable();
}

LL_REGISTER_MOD(
    "InvCleaner",
    "Asynchronously cleans items from all players.",
    "1.0.0",
    {{"author", "Puce"}}
);
