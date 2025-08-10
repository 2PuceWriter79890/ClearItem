#include "mod/InvCleaner.h"

#include <ll/api/command/Command.h>
#include <ll/api/command/CommandHandle.h>
#include <ll/api/command/CommandRegistrar.h>
#include <mc/server/commands/CommandOrigin.h>
#include <mc/server/commands/CommandOutput.h>
#include <mc/nbt/Tag.h>
#include <mc/nbt/CompoundTag.h>
#include <mc/nbt/StringTag.h>
#include <mc/nbt/ShortTag.h>

#include "gmlib/mc/world/actor/Player.h"
#include "gmlib/mc/world/actor/OfflinePlayer.h"

#include <thread>
#include <atomic>

namespace inv_cleaner {

bool InvCleanerMod::load() {
    mLogger.info("InvCleaner Mod 正在加载...");
    return true;
}

bool InvCleanerMod::enable() {
    mLogger.info("InvCleaner Mod 正在启用...");
    registerCommand();
    return true;
}

bool InvCleanerMod::disable() {
    mLogger.info("InvCleaner Mod 正在禁用...");
    return true;
}

void InvCleanerMod::registerCommand() {
    using namespace ll::command;

    auto& command =
        CommandRegistrar::getInstance()
            .getOrCreateCommand(
                "invclean",
                "清理全服玩家背包和末影箱中的指定物品",
                CommandPermissionLevel::Operator
            );

    struct Params {
        std::string itemName;
        std::optional<int> itemAux;
    };
    
    command.overload<Params>(
        [this](CommandOrigin const& origin, CommandOutput& output, Params const& params, Command const&) {
            std::string targetItemName = params.itemName;
            short targetItemAux = params.itemAux.value_or(0);

            std::thread([this, targetItemName, targetItemAux]() {
                this->doClearAllPlayers(targetItemName, targetItemAux);
            }).detach();

            output.success("§a[InvCleaner] 后台清理任务已启动！详细报告将输出至控制台。");
        },
        "itemName",
        "itemAux"
    );
}

void InvCleanerMod::doClearAllPlayers(const std::string& targetItemName, short targetItemAux) {
    mLogger.info("开始全服物品清理任务... 目标: {}:{}", targetItemName, targetItemAux);

    std::atomic<int> processedPlayerCount = 0;
    std::atomic<int> modifiedPlayerCount = 0;

    gmlib::OfflinePlayer::forEachOfflinePlayer([&](gmlib::OfflinePlayer&& player) {
        processedPlayerCount++;

        auto nbtExpected = player.getNbt();
        if (!nbtExpected.has_value()) {
            return true;
        }
        auto playerNbt = nbtExpected.value();

        int inventoryClearedCount = 0;
        int enderChestClearedCount = 0;

        if (auto* inventoryList = playerNbt.try_get_list<CompoundTag>("Inventory")) {
            inventoryClearedCount = clearItemsInList(*inventoryList, targetItemName, targetItemAux);
        }
        if (auto* enderChestList = playerNbt.try_get_list<CompoundTag>("EnderChestInventory")) {
            enderChestClearedCount = clearItemsInList(*enderChestList, targetItemName, targetItemAux);
        }

        if (inventoryClearedCount > 0 || enderChestClearedCount > 0) {
            modifiedPlayerCount++;
            if (!player.setNbt(playerNbt)) {
                mLogger.error("保存玩家 {} 的NBT数据失败！", player.getServerId());
            } else if (auto onlinePlayer = player.getPlayer()) {
                onlinePlayer->sendText("§e[InvCleaner] 您的部分物品已被管理员清理。");
            }
        }
        
        return true;
    });

    mLogger.info("全服物品清理任务完成！");
    mLogger.info("总共处理了 {} 名玩家的数据。", processedPlayerCount.load());
    mLogger.info("从 {} 名玩家的背包/末影箱中清除了指定物品。", modifiedPlayerCount.load());
}

int InvCleanerMod::clearItemsInList(ListTag& itemList, const std::string& targetItemName, short targetItemAux) {
    int clearedCount = 0;
    for (auto it = itemList.begin(); it != itemList.end();) {
        auto* itemTag = dynamic_cast<CompoundTag*>(it->get());
        if (!itemTag) {
            ++it;
            continue;
        }

        auto const* nameTag = itemTag->try_get<StringTag>("Name");
        auto const* damageTag = itemTag->try_get<ShortTag>("Damage");

        if (nameTag && damageTag &&
            nameTag->get() == targetItemName &&
            damageTag->get() == targetItemAux)
        {
            it = itemList.erase(it);
            clearedCount++;
        } else {
            ++it;
        }
    }
    return clearedCount;
}

} // namespace inv_cleaner
