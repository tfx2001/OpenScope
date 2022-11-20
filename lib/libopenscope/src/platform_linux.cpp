/*
 * Copyright 2022 tfx2001 <tfx2001@outlook.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifdef OS_LINUX

#include "lib/platform.h"

#include <gtk/gtk.h>
#include <path_utils.h>

namespace OpenScope {

static void WaitForCleanup() {
    while (gtk_events_pending())
        gtk_main_iteration();
}

bool platform::openFile(std::filesystem::path &path) {
    bool result = true;

    if (!gtk_init_check(nullptr, nullptr)) {
        return false;
    }

    GtkWidget *dialog = gtk_file_chooser_dialog_new(
            "Open File", nullptr, GTK_FILE_CHOOSER_ACTION_OPEN, _("_Cancel"),
            GTK_RESPONSE_CANCEL, _("_Open"), GTK_RESPONSE_ACCEPT, NULL);
    GtkFileFilter *filter = gtk_file_filter_new();
    gtk_file_filter_set_name(filter, "Executable file");
    gtk_file_filter_add_pattern(filter, "*.elf");
    gtk_file_chooser_add_filter(GTK_FILE_CHOOSER(dialog), filter);

    if (gtk_dialog_run(GTK_DIALOG(dialog)) == GTK_RESPONSE_ACCEPT) {
        char *filename;
        filename = gtk_file_chooser_get_filename(GTK_FILE_CHOOSER(dialog));
        path.assign(filename);
    } else {
        result = false;
    }

    WaitForCleanup();
    gtk_widget_destroy(dialog);

    WaitForCleanup();

    return result;
}

} // namespace OpenScope

#endif
