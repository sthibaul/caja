/* vi: set sw=4 ts=4 wrap ai: */
/*
 * fm-group-view.c: This file is part of ____
 *
 * Copyright (C) 2019 yetist <yetist@yetibook>
 *
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 * */

#include <config.h>
#include <string.h>

#include <glib.h>
#include <glib/gi18n.h>
#include <eel/eel-glib-extensions.h>
#include <eel/eel-gtk-macros.h>
#include <eel/eel-vfs-extensions.h>

#include <libcaja-private/caja-file-utilities.h>
#include <libcaja-private/caja-view.h>
#include <libcaja-private/caja-view-factory.h>

#include "fm-group-view.h"

struct _FMGroupView
{
  FMDirectoryView object;
  int             number_of_files;
};

static GList *fm_group_view_get_selection                   (FMDirectoryView   *view);
static GList *fm_group_view_get_selection_for_file_transfer (FMDirectoryView   *view);
static void   fm_group_view_scroll_to_file                  (CajaView *view, const char *uri);
static void   fm_group_view_iface_init                      (CajaViewIface *iface);

G_DEFINE_TYPE_WITH_CODE (FMGroupView, fm_group_view, FM_TYPE_DIRECTORY_VIEW,
                G_IMPLEMENT_INTERFACE (CAJA_TYPE_VIEW, fm_group_view_iface_init));

static void
fm_group_view_add_file (FMDirectoryView *view, CajaFile *file, CajaDirectory *directory)
{
    static GTimer *timer = NULL;
    static gdouble cumu = 0, elaps;
    FM_GROUP_VIEW (view)->number_of_files++;
    GdkPixbuf *icon;

    if (!timer) timer = g_timer_new ();

    g_timer_start (timer);
    icon = caja_file_get_icon_pixbuf (file, caja_get_icon_size_for_zoom_level (CAJA_ZOOM_LEVEL_STANDARD),
                                      TRUE, gtk_widget_get_scale_factor (GTK_WIDGET(view)), 0);

    elaps = g_timer_elapsed (timer, NULL);
    g_timer_stop (timer);

    g_object_unref (icon);

    cumu += elaps;
    g_message ("entire loading: %.3f, cumulative %.3f", elaps, cumu);
}


static void
fm_group_view_begin_loading (FMDirectoryView *view)
{
}

static void
fm_group_view_clear (FMDirectoryView *view)
{
}


static void
fm_group_view_file_changed (FMDirectoryView *view, CajaFile *file, CajaDirectory *directory)
{
}

static GtkWidget *
fm_group_view_get_background_widget (FMDirectoryView *view)
{
    return GTK_WIDGET (view);
}

static GList *
fm_group_view_get_selection (FMDirectoryView *view)
{
    return NULL;
}


static GList *
fm_group_view_get_selection_for_file_transfer (FMDirectoryView *view)
{
    return NULL;
}

static guint
fm_group_view_get_item_count (FMDirectoryView *view)
{
    return FM_GROUP_VIEW (view)->number_of_files;
}

static gboolean
fm_group_view_is_empty (FMDirectoryView *view)
{
    return FM_GROUP_VIEW (view)->number_of_files == 0;
}

static void
fm_group_view_end_file_changes (FMDirectoryView *view)
{
}

static void
fm_group_view_remove_file (FMDirectoryView *view, CajaFile *file, CajaDirectory *directory)
{
    FM_GROUP_VIEW (view)->number_of_files--;
    g_assert (FM_GROUP_VIEW (view)->number_of_files >= 0);
}

static void
fm_group_view_set_selection (FMDirectoryView *view, GList *selection)
{
    fm_directory_view_notify_selection_changed (view);
}

static void
fm_group_view_select_all (FMDirectoryView *view)
{
}

static void
fm_group_view_reveal_selection (FMDirectoryView *view)
{
}

static void
fm_group_view_merge_menus (FMDirectoryView *view)
{
    //EEL_CALL_PARENT (FM_DIRECTORY_VIEW_CLASS, merge_menus, (view));
    FM_DIRECTORY_VIEW_CLASS (fm_group_view_parent_class)->merge_menus(view);
}

static void
fm_group_view_update_menus (FMDirectoryView *view)
{
    FM_DIRECTORY_VIEW_CLASS (fm_group_view_parent_class)->update_menus(view);
}

/* Reset sort criteria and zoom level to match defaults */
static void
fm_group_view_reset_to_defaults (FMDirectoryView *view)
{
}

static void
fm_group_view_bump_zoom_level (FMDirectoryView *view, int zoom_increment)
{
}

static CajaZoomLevel
fm_group_view_get_zoom_level (FMDirectoryView *view)
{
    return CAJA_ZOOM_LEVEL_STANDARD;
}

static void
fm_group_view_zoom_to_level (FMDirectoryView *view,
                             CajaZoomLevel zoom_level)
{
}

static void
fm_group_view_restore_default_zoom_level (FMDirectoryView *view)
{
}

static gboolean
fm_group_view_can_zoom_in (FMDirectoryView *view)
{
    return FALSE;
}

static gboolean
fm_group_view_can_zoom_out (FMDirectoryView *view)
{
    return FALSE;
}

static void
fm_group_view_start_renaming_file (FMDirectoryView *view,
                                   CajaFile *file,
                                   gboolean select_all)
{
}

static void
fm_group_view_click_policy_changed (FMDirectoryView *directory_view)
{
}


static int
fm_group_view_compare_files (FMDirectoryView *view, CajaFile *file1, CajaFile *file2)
{
    if (file1 < file2)
    {
        return -1;
    }

    if (file1 > file2)
    {
        return +1;
    }

    return 0;
}

static gboolean
fm_group_view_using_manual_layout (FMDirectoryView *view)
{
    return FALSE;
}

static void
fm_group_view_end_loading (FMDirectoryView *view,
                           gboolean all_files_seen)
{
}

static void
fm_group_view_finalize (GObject *object)
{
    FMGroupView *group_view;

    group_view = FM_GROUP_VIEW (object);

    G_OBJECT_CLASS (fm_group_view_parent_class)->finalize (object);
}

static void
fm_group_view_emblems_changed (FMDirectoryView *directory_view)
{
}

static char *
fm_group_view_get_first_visible_file (CajaView *view)
{
    return NULL;
}

static void
fm_group_view_scroll_to_file (CajaView *view,
                              const char *uri)
{
}

static void
fm_group_view_grab_focus (CajaView *view)
{
    gtk_widget_grab_focus (GTK_WIDGET (view));
}

static void
fm_group_view_sort_directories_first_changed (FMDirectoryView *view)
{
}

static void
fm_group_view_class_init (FMGroupViewClass *class)
{
    FMDirectoryViewClass *fm_directory_view_class;

    fm_directory_view_class = FM_DIRECTORY_VIEW_CLASS (class);

    G_OBJECT_CLASS (class)->finalize = fm_group_view_finalize;

    fm_directory_view_class->add_file = fm_group_view_add_file;
    fm_directory_view_class->begin_loading = fm_group_view_begin_loading;
    fm_directory_view_class->bump_zoom_level = fm_group_view_bump_zoom_level;
    fm_directory_view_class->can_zoom_in = fm_group_view_can_zoom_in;
    fm_directory_view_class->can_zoom_out = fm_group_view_can_zoom_out;
    fm_directory_view_class->click_policy_changed = fm_group_view_click_policy_changed;
    fm_directory_view_class->clear = fm_group_view_clear;
    fm_directory_view_class->file_changed = fm_group_view_file_changed;
    fm_directory_view_class->get_background_widget = fm_group_view_get_background_widget;
    fm_directory_view_class->get_selection = fm_group_view_get_selection;
    fm_directory_view_class->get_selection_for_file_transfer = fm_group_view_get_selection_for_file_transfer;
    fm_directory_view_class->get_item_count = fm_group_view_get_item_count;
    fm_directory_view_class->is_empty = fm_group_view_is_empty;
    fm_directory_view_class->remove_file = fm_group_view_remove_file;
    fm_directory_view_class->merge_menus = fm_group_view_merge_menus;
    fm_directory_view_class->update_menus = fm_group_view_update_menus;
    fm_directory_view_class->reset_to_defaults = fm_group_view_reset_to_defaults;
    fm_directory_view_class->restore_default_zoom_level = fm_group_view_restore_default_zoom_level;
    fm_directory_view_class->reveal_selection = fm_group_view_reveal_selection;
    fm_directory_view_class->select_all = fm_group_view_select_all;
    fm_directory_view_class->set_selection = fm_group_view_set_selection;
    fm_directory_view_class->compare_files = fm_group_view_compare_files;
    fm_directory_view_class->sort_directories_first_changed = fm_group_view_sort_directories_first_changed;
    fm_directory_view_class->start_renaming_file = fm_group_view_start_renaming_file;
    fm_directory_view_class->get_zoom_level = fm_group_view_get_zoom_level;
    fm_directory_view_class->zoom_to_level = fm_group_view_zoom_to_level;
    fm_directory_view_class->emblems_changed = fm_group_view_emblems_changed;
    fm_directory_view_class->end_file_changes = fm_group_view_end_file_changes;
    fm_directory_view_class->using_manual_layout = fm_group_view_using_manual_layout;
    fm_directory_view_class->end_loading = fm_group_view_end_loading;
}

static const char *
fm_group_view_get_id (CajaView *view)
{
    return FM_GROUP_VIEW_ID;
}


static void
fm_group_view_iface_init (CajaViewIface *iface)
{
    fm_directory_view_init_view_iface (iface);

    iface->get_view_id = fm_group_view_get_id;
    iface->get_first_visible_file = fm_group_view_get_first_visible_file;
    iface->scroll_to_file = fm_group_view_scroll_to_file;
    iface->get_title = NULL;
    iface->grab_focus = fm_group_view_grab_focus;
}


static void
fm_group_view_init (FMGroupView *group_view)
{
}

static CajaView *
fm_group_view_create (CajaWindowSlotInfo *slot)
{
    FMGroupView *view;

    g_assert (CAJA_IS_WINDOW_SLOT_INFO (slot));

    view = g_object_new (FM_TYPE_GROUP_VIEW,
                         "window-slot", slot,
                         NULL);

    return CAJA_VIEW (view);
}

static gboolean
fm_group_view_supports_uri (const char *uri,
                            GFileType file_type,
                            const char *mime_type)
{
    if (g_str_has_prefix (uri, "computer://"))
    {
            return TRUE;
    }
    return FALSE;
}

static CajaViewInfo fm_group_view =
{
    FM_GROUP_VIEW_ID,
    N_("Group View"),
    /* translators: this is used in the view menu */
    N_("_Group View"),
    N_("The group view encountered an error."),
    N_("The group view encountered an error while starting up."),
    N_("Display this location with the group view."),
    fm_group_view_create,
    fm_group_view_supports_uri
};

void
fm_group_view_register (void)
{
    fm_group_view.id = fm_group_view.id;
    fm_group_view.view_combo_label = _(fm_group_view.view_combo_label);
    fm_group_view.view_menu_label_with_mnemonic = _(fm_group_view.view_menu_label_with_mnemonic);
    fm_group_view.error_label = _(fm_group_view.error_label);
    fm_group_view.startup_error_label = _(fm_group_view.startup_error_label);
    fm_group_view.display_location_label = _(fm_group_view.display_location_label);

    caja_view_factory_register (&fm_group_view);
}
