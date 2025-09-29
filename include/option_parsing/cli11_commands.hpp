#pragma once
#include <CLI/CLI.hpp>

namespace cli11_example {

// コールバック方式のサブコマンドを設定
void SetupCallbackCommands(CLI::App &app);

// 手動実行方式のサブコマンドを設定し、実行チェック用のポインタを返す
CLI::App *SetupManualCommands(CLI::App &app);

} // namespace cli11_example
