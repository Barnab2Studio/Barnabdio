QT       += core gui network multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
#    Channel.cpp \
#    ChannelList.cpp \
#    ChannelListController.cpp \
#    ChannelListPanel.cpp \
    MainWindow.cpp \
    User.cpp \
#    UserList.cpp \
#    UserVolumeController.cpp \
#    UserVolumePanel.cpp \
#    VolumeSlideBar.cpp \
    channel.cpp \
    channelcreationdialog.cpp \
    channellistitem.cpp \
    channellistmodel.cpp \
    connectiondialog.cpp \
    globals.cpp \
    main.cpp \
    framelesswindow/framelesswindow.cpp \
    framelesswindow/windowdragger.cpp \
    DarkStyle.cpp \
    playingSoundThread.cpp \
    recordingthread.cpp \
    tcpclient.cpp \
    volumelayout.cpp \
    volumelistlayout.cpp

HEADERS += \
#    Channel.h \
#    ChannelList.h \
#    ChannelListController.h \
#    ChannelListPanel.h \
    MainWindow.h \
    User.h \
#    UserList.h \
#    UserVolumeController.h \
#    UserVolumePanel.h \
#    VolumeSlideBar.h \
    channel.h \
    channelcreationdialog.h \
    channellistitem.h \
    channellistmodel.h \
    connectiondialog.h \
    globals.h \
    playingSoundThread.h \
    recordingthread.h \
    tcpclient.h \
    volumelayout.h \
    volumelistlayout.h
HEADERS     += framelesswindow/framelesswindow.h \
               framelesswindow/windowdragger.h \
               DarkStyle.h

FORMS += \
    channelcreationdialog.ui \
    connectiondialog.ui \
    framelesswindow/framelesswindow.ui \
    mainwindow.ui
    framelesswindow/framelesswindow.ui

INCLUDEPATH += framelesswindow/

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    dark.qss \
    darkstyle/darkstyle.qss \
    darkstyle/icon_branch_closed.png \
    darkstyle/icon_branch_end.png \
    darkstyle/icon_branch_more.png \
    darkstyle/icon_branch_open.png \
    darkstyle/icon_checkbox_checked.png \
    darkstyle/icon_checkbox_checked_disabled.png \
    darkstyle/icon_checkbox_checked_pressed.png \
    darkstyle/icon_checkbox_indeterminate.png \
    darkstyle/icon_checkbox_indeterminate_disabled.png \
    darkstyle/icon_checkbox_indeterminate_pressed.png \
    darkstyle/icon_checkbox_unchecked.png \
    darkstyle/icon_checkbox_unchecked_disabled.png \
    darkstyle/icon_checkbox_unchecked_pressed.png \
    darkstyle/icon_close.png \
    darkstyle/icon_radiobutton_checked.png \
    darkstyle/icon_radiobutton_checked_disabled.png \
    darkstyle/icon_radiobutton_checked_pressed.png \
    darkstyle/icon_radiobutton_unchecked.png \
    darkstyle/icon_radiobutton_unchecked_disabled.png \
    darkstyle/icon_radiobutton_unchecked_pressed.png \
    darkstyle/icon_restore.png \
    darkstyle/icon_undock.png \
    darkstyle/icon_vline.png \
    icons/100x100_broken_image.png \
    icons/16x16_3d_sound.png \
    icons/16x16_about.png \
    icons/16x16_add.png \
    icons/16x16_add_foe.png \
    icons/16x16_add_friend.png \
    icons/16x16_away.png \
    icons/16x16_ban_client.png \
    icons/16x16_ban_list.png \
    icons/16x16_bookmark_add.png \
    icons/16x16_bookmark_add_folder.png \
    icons/16x16_bookmark_duplicate.png \
    icons/16x16_bookmark_manager.png \
    icons/16x16_bookmark_remove.png \
    icons/16x16_capture.png \
    icons/16x16_change_nickname.png \
    icons/16x16_changelog.png \
    icons/16x16_channel_chat.png \
    icons/16x16_channel_collapse_all.png \
    icons/16x16_channel_commander.png \
    icons/16x16_channel_create.png \
    icons/16x16_channel_create_sub.png \
    icons/16x16_channel_delete.png \
    icons/16x16_channel_edit.png \
    icons/16x16_channel_expand_all.png \
    icons/16x16_channel_green.png \
    icons/16x16_channel_green_subscribed.png \
    icons/16x16_channel_private.png \
    icons/16x16_channel_red.png \
    icons/16x16_channel_red_subscribed.png \
    icons/16x16_channel_switch.png \
    icons/16x16_channel_yellow.png \
    icons/16x16_channel_yellow_subscribed.png \
    icons/16x16_check_update.png \
    icons/16x16_connect.png \
    icons/16x16_copy.png \
    icons/16x16_default.png \
    icons/16x16_delete.png \
    icons/16x16_delete_avatar.png \
    icons/16x16_disconnect.png \
    icons/16x16_down.png \
    icons/16x16_edit.png \
    icons/16x16_edit_friend_foe_status.png \
    icons/16x16_emoticon.png \
    icons/16x16_error.png \
    icons/16x16_filetransfer.png \
    icons/16x16_find.png \
    icons/16x16_guisetup.png \
    icons/16x16_hardware_input_muted.png \
    icons/16x16_hardware_output_muted.png \
    icons/16x16_hotkeys.png \
    icons/16x16_identity_manager.png \
    icons/16x16_info.png \
    icons/16x16_input_muted.png \
    icons/16x16_input_muted_local.png \
    icons/16x16_is_talker.png \
    icons/16x16_kick_from_channel.png \
    icons/16x16_kick_from_server.png \
    icons/16x16_listview.png \
    icons/16x16_message_incoming.png \
    icons/16x16_message_info.png \
    icons/16x16_message_outgoing.png \
    icons/16x16_moderated.png \
    icons/16x16_music.png \
    icons/16x16_myts_account.png \
    icons/16x16_myts_account_error.png \
    icons/16x16_myts_account_offline.png \
    icons/16x16_myts_account_online.png \
    icons/16x16_new_chat.png \
    icons/16x16_notifications.png \
    icons/16x16_offline_messages.png \
    icons/16x16_on_whisperlist.png \
    icons/16x16_output_muted.png \
    icons/16x16_permission_overview.png \
    icons/16x16_permissions_channel_client.png \
    icons/16x16_permissions_channel_groups.png \
    icons/16x16_permissions_channels.png \
    icons/16x16_permissions_clients.png \
    icons/16x16_permissions_server_groups.png \
    icons/16x16_phoneticsnickname.png \
    icons/16x16_ping_1.png \
    icons/16x16_ping_2.png \
    icons/16x16_ping_3.png \
    icons/16x16_ping_4.png \
    icons/16x16_ping_calculating.png \
    icons/16x16_ping_disconnected.png \
    icons/16x16_play.png \
    icons/16x16_playback.png \
    icons/16x16_player_chat.png \
    icons/16x16_player_commander_off.png \
    icons/16x16_player_commander_on.png \
    icons/16x16_player_off.png \
    icons/16x16_player_on.png \
    icons/16x16_player_whisper.png \
    icons/16x16_plugins.png \
    icons/16x16_poke.png \
    icons/16x16_present.png \
    icons/16x16_quit.png \
    icons/16x16_recording_start.png \
    icons/16x16_recording_stop.png \
    icons/16x16_register.png \
    icons/16x16_remove_foe.png \
    icons/16x16_remove_friend.png \
    icons/16x16_request_talk_power.png \
    icons/16x16_request_talk_power_cancel.png \
    icons/16x16_revoke_talker.png \
    icons/16x16_send_complaint.png \
    icons/16x16_server_green.png \
    icons/16x16_server_log.png \
    icons/16x16_server_query.png \
    icons/16x16_settings.png \
    icons/16x16_setup_wizard.png \
    icons/16x16_stop.png \
    icons/16x16_subscribe_to_all_channels.png \
    icons/16x16_subscribe_to_channel.png \
    icons/16x16_sync_disable.png \
    icons/16x16_sync_enable.png \
    icons/16x16_temp_server_password.png \
    icons/16x16_temp_server_password_add.png \
    icons/16x16_textformat.png \
    icons/16x16_textformat_bold.png \
    icons/16x16_textformat_foreground.png \
    icons/16x16_textformat_italic.png \
    icons/16x16_textformat_underline.png \
    icons/16x16_token.png \
    icons/16x16_unsubscribe_from_all_channels.png \
    icons/16x16_unsubscribe_from_channel.png \
    icons/16x16_up.png \
    icons/16x16_upload_avatar.png \
    icons/16x16_urlcatcher.png \
    icons/16x16_virtualserver_edit.png \
    icons/16x16_volume.png \
    icons/16x16_warning.png \
    icons/16x16_weblist.png \
    icons/16x16_whisperlists.png \
    icons/200x200_loading_image.png \
    icons/20x20_3d_sound_me.png \
    icons/20x20_3d_sound_user.png \
    icons/24x24_3d_sound.png \
    icons/24x24_activate_microphone.png \
    icons/24x24_away.png \
    icons/24x24_ban_client.png \
    icons/24x24_bookmark_manager.png \
    icons/24x24_capture.png \
    icons/24x24_channel_create.png \
    icons/24x24_channel_create_sub.png \
    icons/24x24_channel_delete.png \
    icons/24x24_connect.png \
    icons/24x24_disconnect.png \
    icons/24x24_edit_friend_foe_status.png \
    icons/24x24_filetransfer.png \
    icons/24x24_hardware_input_muted.png \
    icons/24x24_hardware_output_muted.png \
    icons/24x24_hoster_button.png \
    icons/24x24_iconsview.png \
    icons/24x24_identity_add.png \
    icons/24x24_identity_default.png \
    icons/24x24_identity_export.png \
    icons/24x24_identity_import.png \
    icons/24x24_identity_manager.png \
    icons/24x24_identity_remove.png \
    icons/24x24_input_muted.png \
    icons/24x24_input_muted_local.png \
    icons/24x24_listview.png \
    icons/24x24_message_incoming.png \
    icons/24x24_message_info.png \
    icons/24x24_message_outgoing.png \
    icons/24x24_output_muted.png \
    icons/24x24_permission_channel_group.png \
    icons/24x24_permission_channels.png \
    icons/24x24_permission_client.png \
    icons/24x24_permission_server_group.png \
    icons/24x24_playback.png \
    icons/24x24_player_off.png \
    icons/24x24_player_on.png \
    icons/24x24_player_whisper.png \
    icons/24x24_selectfolder.png \
    icons/24x24_subscribe_to_all_channels.png \
    icons/24x24_unsubscribe_from_all_channels.png \
    icons/24x24_urlcatcher.png \
    icons/24x24_whisperlists.png \
    icons/32x32_addon.png \
    icons/32x32_addon_browser.png \
    icons/32x32_addon_colection.png \
    icons/32x32_addon_collection.png \
    icons/32x32_addon_iconpack.png \
    icons/32x32_addon_soundpack.png \
    icons/32x32_addon_theme.png \
    icons/32x32_addon_translation.png \
    icons/32x32_away.png \
    icons/32x32_broken_image.png \
    icons/32x32_capture.png \
    icons/32x32_channel_create.png \
    icons/32x32_file_home.png \
    icons/32x32_file_refresh.png \
    icons/32x32_file_up.png \
    icons/32x32_guisetup.png \
    icons/32x32_hardware_input_muted.png \
    icons/32x32_hardware_output_muted.png \
    icons/32x32_hotkeys.png \
    icons/32x32_input_muted.png \
    icons/32x32_input_muted_local.png \
    icons/32x32_myts_account.png \
    icons/32x32_notifications.png \
    icons/32x32_output_muted.png \
    icons/32x32_playback.png \
    icons/32x32_player_chat.png \
    icons/32x32_player_off.png \
    icons/32x32_player_on.png \
    icons/32x32_player_whisper.png \
    icons/32x32_reload.png \
    icons/32x32_security.png \
    icons/32x32_sync.png \
    icons/32x32_toggle_block_incoming_whispers.png \
    icons/32x32_toggle_server_query_clients.png \
    icons/32x32_whisperlists.png \
    icons/7x5_down_arrow.png \
    icons/7x5_up_arrow.png \
    icons/8x7_tab_close_button.png \
    icons/emoticons/cool.png \
    icons/emoticons/emoticons.txt \
    icons/emoticons/sad.png \
    icons/emoticons/smile.png \
    icons/emoticons/twinkle.png \
    icons/group_100.png \
    icons/group_200.png \
    icons/group_300.png \
    icons/group_500.png \
    icons/group_600.png \
    icons/warning_external_link.png

RESOURCES += \
    darkstyle.qrc \
    framelesswindow.qrc \
    resources.qrc \
    resources.qrc \


#INCLUDEPATH += "$$PWD/3rdparty/opus/include"
#LIBS += -L"$$PWD/3rdparty/opus/lib/" -lopus
#DESTDIR = /tmp/$${TARGET}/bin
##TARGET = Barnabdio

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/3rdparty/opus/lib/ -lopus
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/3rdparty/opus/lib/ -lopus
#else:unix: LIBS += -L$$PWD/3rdparty/opus/lib/ -lopus

#LIBS += -L$$PWD/3rdparty/opus/lib/ -lopus

#INCLUDEPATH += $$PWD/3rdparty/opus/include
#DEPENDPATH += $$PWD/3rdparty/opus/include
