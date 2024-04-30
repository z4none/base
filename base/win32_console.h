#pragma once

#include <windows.h>
#include <stdio.h>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include <fstream>

//
void RedirectIOToConsole(bool bindStdIn = true, bool bindStdOut = true, bool bindStdErr = true);