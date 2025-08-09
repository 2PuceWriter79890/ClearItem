#include "mod/InvCleaner.h"
#include <ll/api/plugin/RegisterHelper.h>

LL_PLUGIN_ENTRY_HOOK(
    inv_cleaner_main,
    ll::plugin::NativePlugin&,
    ll::plugin::PluginLoadReason reason
) {
    if (reason == ll::plugin::PluginLoadReason::Load) {
        inv_cleaner::InvCleanerMod::getInstance().load();
    }
    return true;
}

LL_PLUGIN_ENABLE_HOOK(inv_cleaner_main, ll::plugin::NativePlugin&) {
    inv_cleaner::InvCleanerMod::getInstance().enable();
    return true;
}

LL_PLUGIN_DISABLE_HOOK(inv_cleaner_main, ll::plugin::NativePlugin&) {
    inv_cleaner::InvCleanerMod::getInstance().disable();
    return true;
}

LL_REGISTER_PLUGIN(
    "InvCleaner",
    "Asynchronously cleans items from all players.",
    "1.0.0",
    {{"author", "Puce"}}
);
