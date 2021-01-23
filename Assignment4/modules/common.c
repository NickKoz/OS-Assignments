#include <common.h>

bool verbose = false;
bool deleted = false;
bool links = false;

int bytes_copied = 0;
double seconds_for_copy = 0.0;

int entities = 1; // root directory included
int entities_copied = 0;
