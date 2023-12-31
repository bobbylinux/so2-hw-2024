# makefile per compilare la versione single process

# definizione del compilatore
CC = gcc

# opzioni di compilazione
CFLAGS = -Wall -Wextra -g

# dipendenze comuni
COMMON_HEADERS = common/common.h


# sorgenti specifici per la versione single process
SP_SOURCES = single-process-version/hw2024sp.c \
             common/list_operations.c \
             common/read_csv_and_build_list.c \
             common/round_down_to_decimal.c \
             common/read_text_file.c \
             common/to_lower.c \
             common/write_output.c \
             common/write_text_file.c

# nome dell'eseguibile single process
SP_EXECUTABLE = so2_hw_2024_sp

# sorgenti specifici per la versione multi-process
MP_SOURCES = multi-process-version/hw2024mp.c \
             multi-process-version/deserialize_list.c \
             multi-process-version/serialize_list.c \
             common/list_operations.c \
             common/read_csv_and_build_list.c \
             common/read_text_file.c \
             common/round_down_to_decimal.c \
             common/to_lower.c \
             common/write_output.c \
             common/write_text_file.c

# nome dell'eseguibile multi-process
MP_EXECUTABLE = so2_hw_2024_mp

# target principale
all: $(SP_EXECUTABLE) $(MP_EXECUTABLE)

# regola per l'eseguibile single process
$(SP_EXECUTABLE): $(COMMON_HEADERS) $(SP_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# regola per l'eseguibile multi-process
$(MP_EXECUTABLE): $(COMMON_HEADERS) $(MP_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^

# pulizia dei file generati
clean:
	rm -f $(SP_EXECUTABLE) $(MP_EXECUTABLE)
