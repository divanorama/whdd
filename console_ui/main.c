#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "libdevcheck.h"

DC_Ctx *dc_ctx;

int main() {
    int r;
    // init libdevcheck
    dc_ctx = dc_init();
    assert(dc_ctx);
    // get list of devices
    DC_DevList *devlist = dc_dev_list(dc_ctx);
    assert(devlist);
    // show list of devices
    int devs_num = dc_dev_list_size(devlist);
    if (devs_num == 0) { printf("No devices found\n"); return 0; }

    int i;
    for (i = 0; i < devs_num; i++) {
        DC_Dev *dev = dc_dev_list_get_entry(devlist, i);
        printf(
                "#%d:" // index
                " %s" // /dev/name
                " %s" // model name
                // TODO human-readable size
                " %"PRIu64" bytes" // size
                "\n"
                ,i
                ,dev->dev_fs_name
                ,dev->model_str
                ,dev->capacity
              );
    }
    printf("Choose device by #:\n");
    int chosen_dev_ind;
    r = scanf("%d", &chosen_dev_ind);
    if (r != 1) {
        printf("Wrong input for device index\n");
        return 1;
    }
    DC_Dev *chosen_dev = dc_dev_list_get_entry(devlist, chosen_dev_ind);
    if (!chosen_dev) {
        printf("No device with index %d\n", chosen_dev_ind);
        return 1;
    }

    // print actions list
    printf("\nChoose action #:\n"
            "1) Show SMART attributes\n"
          );
    int chosen_action_ind;
    r = scanf("%d", &chosen_action_ind);
    if (r != 1) {
        printf("Wrong input for action index\n");
        return 1;
    }
    if (chosen_action_ind == 1) {
        char *text;
        text = dc_dev_smartctl_text(chosen_dev, "-A -i");
        if (text)
            printf("%s\n", text);
        free(text);
    }

    return 0;
}