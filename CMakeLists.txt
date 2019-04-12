cmake_minimum_required(VERSION 3.0)
project(NWP_myftp_2018)

set(CMAKE_C_STANDARD 99)

include_directories(includes)

add_executable(myftp    includes/ftp.h
                        sources/main.c sources/commands/user.c includes/commands.h sources/ftp_command.c includes/user.h sources/str_clear.c sources/count_to_split.c sources/my_str_to_array.c sources/commands/quit.c includes/ftp_message.h sources/commands/pass.c sources/commands/pwd.c sources/commands/cdup.c sources/commands/cwd.c sources/commands/noop.c sources/commands/pasv.c sources/commands/port.c sources/commands/list.c sources/data_transfer.c sources/commands/dele.c sources/commands/retr.c sources/commands/stor.c sources/commands/syst.c sources/commands/help.c sources/server.c sources/utils.c)