// SPDX-FileCopyrightText: 2025 UnionTech Software Technology Co., Ltd.
//
// SPDX-License-Identifier: GPL-3.0-or-later

#include "opslogexport.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <sys/stat.h>
#include <unistd.h>

using namespace std;

OpsLogExport::OpsLogExport(const string &target, const string &home)
    : target_dir(target)
    , home_dir(home)
{
}

void OpsLogExport::run()
{
    // 注意：需要优先创建目录结构
    createDirStruct();

    exportAppLogs();
    exportSystemLogs();
    exportKernelLogs();
    exportDDELogs();
    exportHWLogs();
    exportOSVersionLogs();
    exportDebVersionLogs();

    // 递归设置目录及文件的权限
    system(("chmod -R 777 " + target_dir).c_str());
}

bool OpsLogExport::path_exists(const string &path)
{
    struct stat buffer;
    return (stat(path.c_str(), &buffer) == 0);
}

bool OpsLogExport::create_directories(const string &path)
{
    string cmd = "mkdir -p " + path;
    return system(cmd.c_str()) == 0;
}

void OpsLogExport::copy_file_or_dir(const string &src, const string &dst_dir)
{
    if (!path_exists(src)) return;
    string cmd = "cp -rf " + src + " " + dst_dir + " 2>/dev/null";
    system(cmd.c_str());
}

void OpsLogExport::execute_command(const string &cmd, const string &output_file)
{
    string full_cmd = cmd + " >> " + output_file;
    system(full_cmd.c_str());
}

void OpsLogExport::createDirStruct()
{
    // 创建目录结构
    vector<string> dirs = {
        target_dir + "/kernel",
        target_dir + "/system",
        target_dir + "/dde",
        target_dir + "/app",
        target_dir + "/app/deepin-defender",
        target_dir + "/app/deepin-cloud-print",
        target_dir + "/app/deepin-cloud-scan",
        target_dir + "/app/dde-printer",
        target_dir + "/app/deepin-graphics-driver-manager",
        target_dir + "/app/deepin-boot-maker",
        target_dir + "/app/deepin-scaner",
        target_dir + "/app/kms",
        target_dir + "/app/deepin-compressor",
        target_dir + "/app/dde-calendar",
        target_dir + "/app/deepin-manual",
        target_dir + "/app/deepin-reader",
        target_dir + "/app/deepin-font-manager",
        target_dir + "/app/deepin-deb-installer",
        target_dir + "/app/deepin-terminal",
        target_dir + "/app/deepin-voice-note",
        target_dir + "/app/deepin-devicemanager",
        target_dir + "/app/uos-service-support",
        target_dir + "/app/uos-remote-assistance",
        target_dir + "/app/deepin-system-monitor",
        target_dir + "/app/deepin-editor",
        target_dir + "/app/deepin-calculator",
        target_dir + "/app/deepin-mail",
        target_dir + "/app/deepin-screen-recorder",
        target_dir + "/app/deepin-draw",
        target_dir + "/app/deepin-music",
        target_dir + "/app/deepin-image-viewer",
        target_dir + "/app/deepin-album",
        target_dir + "/app/deepin-movie",
        target_dir + "/app/deepin-camera",
        target_dir + "/app/chineseime",
        target_dir + "/app/deepin-installer",
        target_dir + "/app/deepin-recovery",
        target_dir + "/app/oem-custom",
        target_dir + "/app/uos-activator",
        target_dir + "/app/uos-activator/log",
        target_dir + "/app/fcitx",
        target_dir + "/app/deepin-diskmanager",
        target_dir + "/app/downloader",
        target_dir + "/app/kwin",
        target_dir + "/app/kbox",
        target_dir + "/app/deepin-log-viewer",
        target_dir + "/dde/dde-desktop",
        target_dir + "/dde/dde-file-manager",
        target_dir + "/dde/dde-dock",
        target_dir + "/system/pulseaudio"
    };

    for (const auto& dir : dirs) {
        create_directories(dir);
    }
}

void OpsLogExport::exportAppLogs()
{
    // 安全中心
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-defender/deepin-defender.log", target_dir + "/app/deepin-defender/");
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-defender-daemon/deepin-defender-daemon.log", target_dir + "/app/deepin-defender/");
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-defender-datainterface/deepin-defender-datainterface.log", target_dir + "/app/deepin-defender/");
    // 云打印
    copy_file_or_dir(home_dir + "/.cache/uniontech/deepin-cloud-print/deepin-cloud-print.log", target_dir + "/app/deepin-cloud-print/");
    copy_file_or_dir("/var/log/cups/dcp_log", target_dir + "/app/deepin-cloud-print/");
    copy_file_or_dir(home_dir + "/.cache/uniontech/deepin-cloud-print-configurator/deepin-cloud-print-configurator.log", target_dir + "/app/deepin-cloud-print/");
    // 云扫描
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-cloud-scan/deepin-cloud-scan.log", target_dir + "/app/deepin-cloud-scan/");
    // 打印管理器
    copy_file_or_dir("/var/log/cups/error_log", target_dir + "/app/dde-printer/");
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-printer/dde-printer.log", target_dir + "/app/dde-printer/");
    // 显卡驱动管理器
    copy_file_or_dir("/var/log/deepin-graphics-driver-manager-server.log", target_dir + "/app/deepin-graphics-driver-manager/");
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-graphics-driver-manager/deepin-graphics-driver-manager.log", target_dir + "/app/deepin-graphics-driver-manager/");
    // 启动盘制作工具
    copy_file_or_dir("/var/log/deepin/deepin-boot-maker-service.log", target_dir + "/app/deepin-boot-maker/");
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-boot-maker/deepin-boot-maker.log", target_dir + "/app/deepin-boot-maker/");
    // 扫描管理
    copy_file_or_dir(home_dir + "/.cache/deepin/org.deepin.scanner/org.deepin.scanner", target_dir + "/app/deepin-scaner/");
    // KMS项目
    copy_file_or_dir(home_dir + "/.cache/deepin/kmsclient", target_dir + "/app/kms/");
    copy_file_or_dir(home_dir + "/.cache/deepin/kmstools", target_dir + "/app/kms/");
    // 归档管理器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-compressor/deepin-compressor.log", target_dir + "/app/deepin-compressor/");
    // 日历
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-calendar-service/dde-calendar-service.log", target_dir + "/app/dde-calendar/");
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-calendar/dde-calendar.log", target_dir + "/app/dde-calendar/");
    // 帮助手册
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-manual/deepin-manual.log", target_dir + "/app/deepin-manual/");
    // 文档查看器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-reader/deepin-reader.log", target_dir + "/app/deepin-reader/");
    // 字体管理器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-font-manager/deepin-font-manager.log", target_dir + "/app/deepin-font-manager/");
    // 软件包安装器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-deb-installer/deepin-deb-installer.log", target_dir + "/app/deepin-deb-installer/");
    // 终端
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-terminal/deepin-terminal.log", target_dir + "/app/deepin-terminal/");
    // 语音记事本
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-voice-note/deepin-voice-note.log", target_dir + "/app/deepin-voice-note/");
    // 设备管理器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-devicemanager/deepin-devicemanager.log", target_dir + "/app/deepin-devicemanager/");
    // 服务与支持
    copy_file_or_dir(home_dir + "/.cache/deepin/uos-service-support/uos-service-support.log", target_dir + "/app/uos-service-support/");
    // 远程协助
    copy_file_or_dir(home_dir + "/.cache/deepin/uos-remote-assistance/uos-remote-assistance.log", target_dir + "/app/uos-remote-assistance/");
    // 系统监视器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-system-monitor/deepin-system-monitor.log", target_dir + "/app/deepin-system-monitor/");
    // 文本编辑器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-editor/deepin-editor.log", target_dir + "/app/deepin-editor/");
    // 计算器
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-calculator/deepin-calculator.log", target_dir + "/app/deepin-calculator/");
    // 邮箱
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-mail/deepin-mail.log", target_dir + "/app/deepin-mail/");
    // 截图录屏
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-screen-recorder/deepin-screen-recorder.log", target_dir + "/app/deepin-screen-recorder/");
    // 画板
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-draw/deepin-draw.log", target_dir + "/app/deepin-draw/");
    // 音乐
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-music/deepin-music.log", target_dir + "/app/deepin-music/");
    // 看图
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-image-viewer/deepin-image-viewer.log", target_dir + "/app/deepin-image-viewer/");
    // 相册
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-album/deepin-album.log", target_dir + "/app/deepin-album/");
    // 影院
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-movie/deepin-movie.log", target_dir + "/app/deepin-movie/");
    // 相机
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-camera/deepin-camera.log", target_dir + "/app/deepin-camera/");
    // 中文输入法
    copy_file_or_dir(home_dir + "/.cache/org.deepin.chineseime/ime/chineseime-qimpanel.log", target_dir + "/app/chineseime/");
    copy_file_or_dir(home_dir + "/.cache/org.deepin.chineseime/ime/fcitx-iflyime.log", target_dir + "/app/chineseime/");
    copy_file_or_dir(home_dir + "/.cache/org.deepin.chineseime/ime/ossp.log", target_dir + "/app/chineseime/");
    // 安装器
    copy_file_or_dir("/var/log/deepin-installer.log", target_dir + "/app/deepin-installer/");
    copy_file_or_dir("/var/log/deepin-installer-first-boot.log", target_dir + "/app/deepin-installer/");
    // 备份还原
    copy_file_or_dir("/recovery/deepin-recovery-gui.log", target_dir + "/app/deepin-recovery/");
    copy_file_or_dir("/recovery/deepin-recovery-gui-old.log", target_dir + "/app/deepin-recovery/");
    // ISO定制工具
    copy_file_or_dir("/var/local/oem-custom-tool/oem-custom-tool.log", target_dir + "/app/oem-custom/");
    copy_file_or_dir("/var/local/oem-custom-tool/oem-custom-tool-bk.log", target_dir + "/app/oem-custom/");
    copy_file_or_dir("/root/.cache/isocustomizer-agent/iso-customizer-agent/iso-customizer-agent.log", target_dir + "/app/oem-custom/");
    // 授权管理客户端
    copy_file_or_dir(home_dir + "/.cache/uos/uos-activator", target_dir + "/app/uos-activator/");
    copy_file_or_dir(home_dir + "/.cache/uos/uos-activator-cmd", target_dir + "/app/uos-activator/");
    copy_file_or_dir(home_dir + "/.cache/uos-agent/uos-license-agent", target_dir + "/app/uos-activator/");
    copy_file_or_dir(home_dir + "/.cache/uos-agent/uos-activator-kms", target_dir + "/app/uos-activator/");
    // 授权管理客户端(1020及之后版本日志)
    copy_file_or_dir("/var/log/uos/uos-license-agent", target_dir + "/app/uos-activator/log/");
    copy_file_or_dir("/var/log/uos/uos-activator-kms", target_dir + "/app/uos-activator/log/");
    // 输入法配置
    //    system(("cp -rf /tmp/fcitx*.log " + target_dir + "/app/fcitx/").c_str());
    // 磁盘管理器
    copy_file_or_dir("/var/log/deepin/deepin-diskmanager-service/Log", target_dir + "/app/deepin-diskmanager/");
    // 下载器
    copy_file_or_dir(home_dir + "/.config/uos/downloader/Log", target_dir + "/app/downloader/");
    // 窗口管理器(查看内核显卡驱动、查看核外驱动、查看窗管版本)
    execute_command("lspci -v |grep VGA -A19", target_dir + "/app/kwin/lspci_VGA.log");
    //    execute_command("glxinfo -B", target_dir + "/app/kwin/glxinfo.log");
    execute_command("apt policy kwin-x11 dde-kwin", target_dir + "/app/kwin/kwin_info.log");
    // 安卓容器
    copy_file_or_dir("/usr/share/log/log.txt", target_dir + "/app/kbox/");
    copy_file_or_dir(home_dir + "/log/AospLog.log", target_dir + "/app/kbox/");
    copy_file_or_dir(home_dir + "/log/KboxServer.log", target_dir + "/app/kbox/");
    // 日志收集工具
    copy_file_or_dir(home_dir + "/.cache/deepin/deepin-log-viewer/deepin-log-viewer.log", target_dir + "/app/deepin-log-viewer/");
    copy_file_or_dir("/var/log/deepin/deepin-log-viewer-service", target_dir + "/app/deepin-log-viewer/");
}

void OpsLogExport::exportSystemLogs()
{
    // 系统
    execute_command("dmesg -T", target_dir + "/system/dmesg_system.log");
    execute_command("journalctl", target_dir + "/system/journalctl.log");
    execute_command("journalctl -b -0", target_dir + "/system/journalctl-0.log");
    execute_command("journalctl -b -1", target_dir + "/system/journalctl-1.log");
    execute_command("journalctl -xb", target_dir + "/system/journalctl-xb.log");
    copy_file_or_dir("/var/log/apt/history.log", target_dir + "/system/");
    copy_file_or_dir("/var/log/alternatives.log", target_dir + "/system/");
    copy_file_or_dir("/var/log/kern.log", target_dir + "/system/");
    copy_file_or_dir("/var/log/syslog", target_dir + "/system/");
    copy_file_or_dir("/var/log/dpkg.log", target_dir + "/system/");
    // xorg /var/log/目录下所有log文件
    system(("cp -rf /var/log/Xorg* " + target_dir + "/system/ 2>/dev/null").c_str());
    // pulse　audio /home/uos/pulse.log
    copy_file_or_dir(home_dir + "/pulse.log", target_dir + "/system/pulseaudio/");
}

void OpsLogExport::exportKernelLogs()
{
    // 内核
    copy_file_or_dir("/etc/product-info", target_dir + "/kernel/");
    execute_command("uname -a", target_dir + "/kernel/uname-a.log");
    execute_command("dmesg", target_dir + "/kernel/dmesg.log");
    copy_file_or_dir("/sys/firmware/acpi/tables/DSDT", target_dir + "/kernel/");
    copy_file_or_dir("/var/log/lightdm/lightdm.log", target_dir + "/kernel/");
    system(("cp -rf /var/log/Xorg.0.log* " + target_dir + "/kernel/ 2>/dev/null").c_str());
    execute_command("lspci -vvv | grep -A 12 'VGA c'", target_dir + "/kernel/lspci_VGA.log");
    //    execute_command("ifconfig", target_dir + "/kernel/ifconfig.log");
    //    execute_command("ethtool -i $(ifconfig | grep --max-count=1 ^en | awk -F ':' '{print $1}')", target_dir + "/kernel/eth_info.log");
    execute_command("dmesg | grep iwlwifi", target_dir + "/kernel/dmesg_network.log");
    execute_command("journalctl --system", target_dir + "/kernel/journalctl_system.log");
    // ⽆法识别声卡问题⽇志
    //    execute_command("aplay -l", target_dir + "/kernel/aplay.log");
    execute_command("lshw -c sound", target_dir + "/kernel/sound_info.log");
    // 龙芯内核
    copy_file_or_dir("/var/log/kern.log", target_dir + "/kernel/");
}

void OpsLogExport::exportDDELogs()
{
    // 文件管理器
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-desktop/dde-desktop.log", target_dir + "/dde/dde-desktop/");
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-file-manager/dde-file-manager.log", target_dir + "/dde/dde-file-manager/");
    copy_file_or_dir(home_dir + "/.cache/deepin/dde-dock/dde-dock.log", target_dir + "/dde/dde-dock/");
    copy_file_or_dir("/var/log/deepin/dde-file-manager-daemon", target_dir + "/dde/dde-file-manager/");
    copy_file_or_dir("/var/log/messages", target_dir + "/dde/");
    copy_file_or_dir("/var/log/syslog", target_dir + "/dde/");
    execute_command("coredumpctl list", target_dir + "/dde/coredumpctl.log");
    execute_command("free -m", target_dir + "/dde/free-m.log");   // 查看内存情况，输出内容截图或保存
    execute_command("udisksctl dump", target_dir + "/dde/udiskctl_dump.txt");
    execute_command("df -h", target_dir + "/dde/df-h.txt");
    // DDE
    system(("cp " + home_dir + "/Desktop/DDE_LOG.zip " + target_dir + "/dde/ 2>/dev/null").c_str());
    copy_file_or_dir("/var/log/journalLog", target_dir + "/dde/");
}

void OpsLogExport::exportHWLogs()
{
    // hw-info
    execute_command("dmidecode -t 0", target_dir + "/system_info.txt");
    execute_command("dmidecode -t 11", target_dir + "/system_info.txt");
    execute_command("cat /etc/hw_version", target_dir + "/system_info.txt");
    execute_command("echo", target_dir + "/system_info.txt");
    execute_command("/usr/sbin/hwfirmware -v", target_dir + "/system_info.txt");
    execute_command("echo", target_dir + "/system_info.txt");
    execute_command("lscpu", target_dir + "/system_info.txt");
}

void OpsLogExport::exportOSVersionLogs()
{
    execute_command("uname -a", target_dir + "/info-version.txt");
    execute_command("echo", target_dir + "/info-version.txt");
    execute_command("cat /etc/os-version", target_dir + "/info-version.txt");
    execute_command("echo", target_dir + "/info-version.txt");
    execute_command("echo -n 开发者模式开启状态：", target_dir + "/info-version.txt");
    execute_command("cat /var/lib/deepin/developer-mode/enabled", target_dir + "/info-version.txt");
    execute_command("echo \"\\n\"", target_dir + "/info-version.txt");
    execute_command("uos-activator-cmd -q", target_dir + "/info-version.txt");
}

void OpsLogExport::exportDebVersionLogs()
{
    execute_command("dpkg -l", target_dir + "/deb-version.txt");
}
