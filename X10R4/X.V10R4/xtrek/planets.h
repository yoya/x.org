/* $Header: planets.h,v 10.1 86/11/30 15:30:38 jg Rel $ */
/* Copyright (c) 1986 	Chris Guthrie */

/*

struct planet {
    int pl_no;
    int pl_status;
    int pl_owner;
    int pl_x;
    int pl_y;
    char pl_name[16];
    int pl_armies;
}
*/

struct planet pdata[MAXPLANETS] = {
    {  0, (FED|PLHOME|PLFUEL|PLREPAIR), FED, 20000, 80000, "Earth", 30, FED },
    {  1, FED, FED, 10000, 65000, "Sasus", 30, FED },
    {  2, FED, FED, 25000, 60000, "Candeleron", 30, FED },
    {  3, FED, FED, 44000, 75000, "Beta III", 30, FED },
    {  4, FED, FED, 33000, 55000, "Janus", 30, FED },
    {  5, FED, FED, 30000, 90000, "Deneb VI", 30, FED },
    {  6, FED, FED, 37000, 70000, "Ceti IV", 30, FED },
    {  7, FED, FED, 11000, 75000, "Altar", 30, FED },
    {  8, FED, FED,  8000, 93000, "Dekar", 30, FED },
    {  9, FED, FED, 13000, 87000, "Daltus", 30, FED },
    { 10, (ROM|PLHOME|PLFUEL|PLREPAIR), ROM, 20000, 20000, "Romulus", 30, ROM },
    { 11, ROM, ROM, 45000,  7000, "Ethen", 30, ROM },
    { 12, ROM, ROM,  4000, 17000, "Amur", 30, ROM },
    { 13, ROM, ROM, 38000, 39000, "Remus", 30, ROM },
    { 14, ROM, ROM, 15000, 35000, "Bal", 30, ROM },
    { 15, ROM, ROM, 28000,  8000, "Tahndar", 30, ROM },
    { 16, ROM, ROM, 28000, 23000, "Dact", 30, ROM },
    { 17, ROM, ROM, 40000, 25000, "Sirius II", 30, ROM },
    { 18, ROM, ROM, 25000, 44000, "Rakhir", 30, ROM },
    { 19, ROM, ROM,  8000, 29000, "Rus", 30, ROM },
    { 20, (KLI|PLHOME|PLFUEL|PLREPAIR), KLI, 80000, 20000, "Klingus", 30, KLI },
    { 21, KLI, KLI, 68000, 44000, "Malatrakir", 30, KLI },
    { 22, KLI, KLI, 60000, 10000, "Amakron", 30, KLI },
    { 23, KLI, KLI, 72000, 29000, "Laltir", 30, KLI },
    { 24, KLI, KLI, 93000,  8000, "Khartair", 30, KLI },
    { 25, KLI, KLI, 90000, 43000, "Monpur III", 30, KLI },
    { 26, KLI, KLI, 61000, 33000, "Sectus", 30, KLI },
    { 27, KLI, KLI, 83000, 37000, "Makus", 30, KLI },
    { 28, KLI, KLI, 68000, 21000, "Jakar", 30, KLI },
    { 29, KLI, KLI, 73000,  5000, "Gendus", 30, KLI },
    { 30, (ORI|PLHOME|PLFUEL|PLREPAIR), ORI, 80000, 80000, "Orion", 30, ORI },
    { 31, ORI, ORI, 92000, 64000, "Amterion", 30, ORI },
    { 32, ORI, ORI, 65000, 55000, "Lumecis", 30, ORI },
    { 33, ORI, ORI, 64000, 75000, "Bitar V", 30, ORI },
    { 34, ORI, ORI, 72000, 69000, "Prastan", 30, ORI },
    { 35, ORI, ORI, 64000, 80000, "Sorlen", 30, ORI },
    { 36, ORI, ORI, 62000, 89000, "Zetus", 30, ORI },
    { 37, ORI, ORI, 91000, 94000, "Jih", 30, ORI },
    { 38, ORI, ORI, 70000, 93000, "Markus II", 30, ORI },
    { 39, ORI, ORI, 85000, 70000, "Oren", 30, ORI }
};
