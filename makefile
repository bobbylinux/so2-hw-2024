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

# directory della GSL
GSL_DIR = gsl

# librerie GSL
GSL_LIBS = -L$(GSL_DIR)/.libs -lgsl -lgslcblas -lm

# include della GSL
GSL_INCLUDE = -I$(GSL_DIR)/include -I$(GSL_DIR)

# target principale
all: $(EXECUTABLE)

# regola per l'eseguibile
$(EXECUTABLE): $(COMMON_HEADERS) $(SP_SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(GSL_INCLUDE) $(GSL_LIBS)

# pulizia dei file generati
clean:
	rm -f $(EXECUTABLE)
