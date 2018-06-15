cJSON_SRC_DIR := $(LIBFARMBOT_VENDER_DIR)/cJSON
cJSON_INCLUDE_DIR := $(LIBFARMBOT_VENDER_DIR)/cJSON

cJSON_SRC := $(wildcard $(cJSON_SRC_DIR)/*.c)
cJSON_OBJ := $(cJSON_SRC:.c=.o)

LIBFARMBOT_OBJ += $(cJSON_OBJ)

$(cJSON_SRC_DIR)/%.o: $(cJSON_SRC_DIR)/%.c
	$(CC) -I$(cJSON_INCLUDE_DIR) -fPIC -c $< -o $@