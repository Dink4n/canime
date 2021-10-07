#ifndef CANIME_COLORS_H
#define CANIME_COLORS_H

/*
 * This is free and unencumbered software released into the public domain.
 *
 * For more information, please refer to <https://unlicense.org>
 */

//Regular text
#define C_BLK "\x1B[0;30m"
#define C_RED "\x1B[0;31m"
#define C_GRN "\x1B[0;32m"
#define C_YEL "\x1B[0;33m"
#define C_BLU "\x1B[0;34m"
#define C_MAG "\x1B[0;35m"
#define C_CYN "\x1B[0;36m"
#define C_WHT "\x1B[0;37m"

//Regular bold text
#define C_BBLK "\x1B[1;30m"
#define C_BRED "\x1B[1;31m"
#define C_BGRN "\x1B[1;32m"
#define C_BYEL "\x1B[1;33m"
#define C_BBLU "\x1B[1;34m"
#define C_BMAG "\x1B[1;35m"
#define C_BCYN "\x1B[1;36m"
#define C_BWHT "\x1B[1;37m"

//Regular underline text
#define C_UBLK "\x1B[4;30m"
#define C_URED "\x1B[4;31m"
#define C_UGRN "\x1B[4;32m"
#define C_UYEL "\x1B[4;33m"
#define C_UBLU "\x1B[4;34m"
#define C_UMAG "\x1B[4;35m"
#define C_UCYN "\x1B[4;36m"
#define C_UWHT "\x1B[4;37m"

//Regular background
#define C_BLKB "\x1B[40m"
#define C_REDB "\x1B[41m"
#define C_GRNB "\x1B[42m"
#define C_YELB "\x1B[43m"
#define C_BLUB "\x1B[44m"
#define C_MAGB "\x1B[45m"
#define C_CYNB "\x1B[46m"
#define C_WHTB "\x1B[47m"

//High intensty background
#define C_BLKHB "\x1B[0;100m"
#define C_REDHB "\x1B[0;101m"
#define C_GRNHB "\x1B[0;102m"
#define C_YELHB "\x1B[0;103m"
#define C_BLUHB "\x1B[0;104m"
#define C_MAGHB "\x1B[0;105m"
#define C_CYNHB "\x1B[0;106m"
#define C_WHTHB "\x1B[0;107m"

//High intensty text
#define C_HBLK "\x1B[0;90m"
#define C_HRED "\x1B[0;91m"
#define C_HGRN "\x1B[0;92m"
#define C_HYEL "\x1B[0;93m"
#define C_HBLU "\x1B[0;94m"
#define C_HMAG "\x1B[0;95m"
#define C_HCYN "\x1B[0;96m"
#define C_HWHT "\x1B[0;97m"

//Bold high intensity text
#define C_BHBLK "\x1B[1;90m"
#define C_BHRED "\x1B[1;91m"
#define C_BHGRN "\x1B[1;92m"
#define C_BHYEL "\x1B[1;93m"
#define C_BHBLU "\x1B[1;94m"
#define C_BHMAG "\x1B[1;95m"
#define C_BHCYN "\x1B[1;96m"
#define C_BHWHT "\x1B[1;97m"

//Reset
#define C_RESET "\x1B[0m"

#endif /* CANIME_COLORS_H */
