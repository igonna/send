#include <string.h>
#include <stdlib.h>
//get file name clean
void gfnc(char *file_name, char* nf_name)
{
	int fname_size = strlen(file_name) + 1;
	
	//next directory character	
	char is_ndc = 'n';
	for (int step = 0; step < fname_size; ++step)
		if (file_name[step] == '/') {
			is_ndc = 'y';
			break;
		}
	free(nf_name);
	if (is_ndc == 'n') {
		nf_name = (char*)malloc(fname_size);
		strcpy(nf_name, file_name);
		return;
	} else {
		//last new file char
		int last_ndc = 0;
		for (int step = 0; step < fname_size; ++step)
			if (file_name[step] == '/') {
				last_ndc = step;
				break;
			}
		int n_str_size = fname_size - last_ndc - 1;
		nf_name = (char*)malloc(n_str_size);
		for (int step = 0; step < n_str_size; ++step)
			nf_name[step] = file_name[1 + last_ndc++];
		nf_name[n_str_size - 1] = '\0';
		return;
	}
		
}
