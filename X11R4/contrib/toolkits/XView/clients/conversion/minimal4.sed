  cat ${1} | \
    sed -e 's%\<xv_get_value%panel_get_value%g' \
        -e 's%\<walkmenu\.h%openmenu\.h%g' \
        -e 's%\<WIN_BOTTOM_MARGIN\>%XV_BOTTOM_MARGIN%g' \
        -e 's%\<WIN_DEVICE_NAME\>%XV_XNAME%g' \
        -e 's%\<WIN_DEVICE_NUMER\>%XV_DEVICE_NUMER%g' \
        -e 's%\<win_environ\.h%win_env\.h%g' \
        -e 's%\<WIN_FD%/* XView CONVERSION - Defunct, see Sect 3\.2 & 4\.3 */WIN_FD%g' \
        -e 's%\<win_fdto%/* XView CONVERSION - Defunct, see Sect 2\.5 */win_fdto%g' \
        -e 's%\<win_findintersect%/* XView CONVERSION - Defunct, use win_pointer_under instead */win_findintersect%g' \
        -e 's%etgfxwindow%etgfxwindow/* XView CONVERSION - Defunct, GFX subwindows are no longer supported See Sect 4\.3 */%g' \
        -e 's%\<win_getcursor\>%/* XView CONVERSION - Defunct, see Sect 2\.3 */win_getcursor%g' \
        -e 's%\<win_getowner\>%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_getowner%g' \
        -e 's%\<win_getparentwindow\>%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_getparentwindow%g' \
        -e 's%\<win_getsavedrect%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_getsavedrect%g' \
        -e 's%\<win_getscreenposition%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_getscreenposition%g' \
        -e 's%\<win_getuserflag%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_getuserflag%g' \
        -e 's%\<win_get_button_order%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_button_order%g' \
        -e 's%\<win_get_designee%/* XView CONVERSION - Possibly Defunct, see Sect 2\.5 */win_get_designee%g' \
        -e 's%\<win_get_event_timeout%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_event_timeout%g' \
        -e 's%\<win_get_fd%/* XView CONVERSION - Defunct, see Sect 2\.3 & 2\.4 */win_get_fd%g' \
        -e 's%\<win_get_focus_event%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_focus_event%g' \
        -e 's%\<win_get_kbd_mask%/* XView CONVERSION - Possibly Defunct, see Sect 4\.3 */win_get_kbd_mask%g' \
        -e 's%\<win_get_pick_mask%/* XView CONVERSION - Possibly Defunct, see Sect 4\.3 */win_get_pick_mask%g' \
        -e 's%\<win_get_pixwin%/* XView CONVERSION - Defunct, see Sect 2\.3 */win_get_pixwin%g' \
        -e 's%\<win_get_scaling%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_scaling%g' \
        -e 's%\<win_get_swallow_event%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_swallow_event%g' \
        -e 's%\<win_get_tree_layer%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_get_tree_layer%g' \
        -e 's%\<win_initscreenfromargv%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_initscreenfromargv%g' \
        -e 's%\<win_insertblanket%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_insertblanket%g' \
        -e 's%\<win_is_input_device%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_is_input_device%g' \
        -e 's%\<win_numbertoname%/* XView CONVERSION - Possibly Defunct, see Sect 2\.5 */win_numbertoname%g' \
        -e 's%\<win_release_event_lock%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_release_event_lock%g' \
        -e 's%\<win_removeblanket%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_removeblanket%g' \
        -e 's%\<win_remove_input_device%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_remove_input_device%g' \
        -e 's%\<win_screendestroy%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_screendestroy%g' \
        -e 's%\<win_screenget%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_screenget%g' \
        -e 's%\<win_screennew%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_screennew%g' \
        -e 's%\<win_setcursor%/* XView CONVERSION - Defunct, see Sect 2\.3 */win_setcursor%g' \
        -e 's%\<win_setkbd%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setkbd%g' \
        -e 's%\<win_setms%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setms%g' \
        -e 's%\<win_setowner%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setowner%g' \
        -e 's%\<win_setparentwindow%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setparentwindow%g' \
        -e 's%\<win_setsavedrect%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setsavedrect%g' \
        -e 's%\<win_setscreenposition%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setscreenposition%g' \
        -e 's%\<win_setuserflag%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_setuserflag%g' \
        -e 's%\<win_set_button_order%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_set_button_order%g' \
        -e 's%\<win_set_designee%/* XView CONVERSION - Defunct, see Sect 2\.5 */win_set_designee%g' \
        -e 's%\<win_set_event_timeout%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_set_event_timeout%g' \
        -e 's%\<win_set_focus_event%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_set_focus_event%g' \
        -e 's%\<win_set_input_device%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_set_input_device%g' \
        -e 's%\<win_set_kbd_mask%/* XView CONVERSION - Possibly Defunct, see Sect 4\.3 */win_set_kbd_mask%g' \
        -e 's%\<in_set_pick_mask%/* XView CONVERSION - Possibly Defunct, see Sect 4\.3 */win_set_pick_mask%g' \
        -e 's%\<win_set_swallow_event%/* XView CONVERSION - Defunct, see Sect 4\.3 */win_set_swallow_event%g' \
	-e 's%\<Window\>%/* XView CONVERSION - Due to a name space clash with Xlib, the SunView data-type Window is now Xv_Window in XView */Window%g' \
        -e 's%\<window_fit\>%/* XView CONVERSION - If attempting to fit the a frame around multiple subwindows where at least one is a text, tty or term subwindow, you should explicitly xv_set the size of the windows first, then call window_fit, see window_fit in documentation */window_fit%g' \
        -e 's%\<window_default_event_proc%/* XView CONVERSION - Defunct, remove reference and write own event proc */window_default_event_proc%g' \
    	-e 's%\<window_create\>%xv_create%g' \
        -e 's%\<window_destroy\>%/* XView CONVERSION - If this is being called from within an event/notify call-back proc, call func xv_destroy_safe instead */xv_destroy%g' \
        -e 's%\<window_get\>%xv_get%g' \
        -e 's%\<window_set\>%xv_set%g' \
        -e 's%\<wmgr_%/* XView CONVERSION - Possibly Defunct, see Sect 4\.3 */wmgr_%g' 
