//Framework

#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
#define LENGTH 50
#define FIELDS_FILE "feilds_names.cfg"
#define DATA_FILE "data_file.dat"
#define MENU_FILE "menu.cfg"
#define CUSTOM_MESSAGE "custommessages.cfg"
char ** getcustommessage();
int fields_counter();
void  showmenu();
void search();
void create();
void display();
int get_record_postion();
void update();
void delete();
void get_id();
char **getfieldarray();
int getmessagefieldcount();

int custom_message_fieldcount;
char given_id[LENGTH];
 char **custom_message;

int field_count;

FILE *fp_custom_message;
FILE *fp_data_file;
FILE *fp_menu_file;
FILE *fp_field_file;

int fields_counter()
{
	fp_field_file  = fopen(FIELDS_FILE, "r");
    field_count = 0;
    rewind(fp_field_file);
    char field_Data[LENGTH];

	while(fgets(field_Data, LENGTH, fp_field_file) != NULL)
	{

      field_count++;
	}
	fclose(fp_field_file);
	return field_count;

}

int getmessagefieldcount()
{

	char array[LENGTH];
   fp_custom_message = fopen(CUSTOM_MESSAGE, "r");
   while(fgets(array, LENGTH, fp_custom_message))
   {
   	custom_message_fieldcount++;
   }
      	printf("%d",custom_message_fieldcount);
   fclose(fp_custom_message);
  return custom_message_fieldcount;
}

 char **getcustommessage()
{
	int counter = 0;
	char array[LENGTH];
	custom_message = malloc(sizeof(char*) * custom_message_fieldcount);
	fp_custom_message = fopen(CUSTOM_MESSAGE, "r");

	while(fgets(array,LENGTH,fp_custom_message))
	{
		custom_message [counter]= malloc(sizeof(char) * LENGTH);
		array[strlen(array) - 1] = '\0';
		strcpy(custom_message[counter], array);
		printf("%s",custom_message[counter]);
		counter++;
	}	   
fclose(fp_custom_message);
return custom_message;
}
char **field_array;


void main()
{
	getmessagefieldcount();
	fields_counter();
	getfieldarray();
	getcustommessage();
  while(1)
  {

	showmenu();
  }   
   
}

void showmenu()
{
	int menu_Size;
	fp_menu_file = fopen(MENU_FILE, "r");

  char menu_data[LENGTH];
  int user_choice;

  while(fgets(menu_data, LENGTH, fp_menu_file))
  {
    	printf("%s", menu_data);
  }
	printf("\nEnter your choice: ");
	scanf("%d", &user_choice);
	switch(user_choice)
	{
		case 1: create();
			      break;
    case 2: display();
    	      break;
    case 3: update();
            break;
    case 4: search();
    	      break;
    case 5: delete();
    	      break;
    case 6: printf("\nExited..");
    	      

    default : printf("Enter a valid choice.");
	}

}
char **getfieldarray()
{
	field_array;
	fields_counter();
	field_array = malloc(sizeof(char*) * field_count);
	int counter = 0;
	char array[LENGTH];
	fp_field_file = fopen(FIELDS_FILE, "r");
	while(fgets(array,LENGTH,fp_field_file))
	{   
        field_array[counter] =  malloc(sizeof(char) * LENGTH);
        array[strlen(array) - 1] = '\0';
		    strcpy(field_array[counter], array);
		    counter++;
	}

	fclose(fp_field_file);
	return field_array;

}


void create()
{   
    int counter;
	  fp_data_file = fopen(DATA_FILE, "a");
	  fputc('A', fp_data_file);
  	fclose(fp_data_file);
  	char field_Data[LENGTH];
    fields_counter();
  	printf("\nfield count %d\n", field_count);
	  getfieldarray();
	  for(counter = 0;counter < field_count; counter++)
	  {    

			printf("Enter %s: ", field_array[counter]);
			scanf(" %[^\n]s", &field_Data);
			fp_data_file = fopen(DATA_FILE, "a");
			fwrite(field_Data, sizeof(char), LENGTH, fp_data_file);
			fclose(fp_data_file);
	  }
     printf("\ndetails added successfully...\n");
    
}


void display()
{
	fp_data_file =  fopen(DATA_FILE, "r");
	int datafilesize, counter = 0;
	char field_Data[LENGTH];

	fseek(fp_data_file, 0 , SEEK_END);
	datafilesize = ftell(fp_data_file);


	rewind(fp_data_file);
	fields_counter();
	getfieldarray();
  printf("\n------------------------Details-------------------\n");
	while(ftell(fp_data_file) < datafilesize)
	{
        char status;
        rewind(fp_field_file);
        counter = 0;
        status = fgetc(fp_data_file);
       // printf("\n%c\n",status);
        if(status == 'A')
        {   
        	while(counter < field_count)
        	{

	        	if(fread(field_Data, sizeof(char), LENGTH, fp_data_file))
	        	{   
	                
	        		printf("%s : %s\n",field_array[counter], field_Data);
	        	    counter++;
	        			        		
	           	}
            }
            printf("\n------------------------------------------------\n");
        }

	}

}


void get_id()
{
	printf("\n Enter id number: ");
	scanf("%s", &given_id);
}


int get_record_postion()
{
	get_id();
	int len, found = 0;
	int datafilesize;
  fp_data_file = fopen(DATA_FILE,"r");

	fseek(fp_data_file, 0 , SEEK_END);
	datafilesize = ftell(fp_data_file);

	printf("%d",datafilesize);

	rewind(fp_data_file);
	fields_counter();
	getfieldarray();
	while(ftell(fp_data_file) < datafilesize)
	{

		char status;
		char field_Data[LENGTH];
		status = fgetc(fp_data_file);
		rewind(fp_field_file);
	    if(status == 'A')
	    {

					fread(field_Data, sizeof(char), LENGTH, fp_data_file);
					
						if(strcmp(field_Data, given_id) == 0)
						{
							return ftell(fp_data_file) - LENGTH;
						}
						else
						{
			            fseek(fp_data_file, (field_count - 1) * LENGTH, SEEK_CUR);

						}
					

	    }
	    else

	    {       
           fseek(fp_data_file, field_count * LENGTH, SEEK_CUR);

	    }
	}
	return -1;

}


void search()
{
  int position = get_record_postion();
  char field_Data[LENGTH];
  if(position != -1)
   {
  		printf("\n %s %s\n",field_array[0], custom_message[1]);
  		int counter=0;
  		fp_data_file = fopen(DATA_FILE, "r");
  		fseek(fp_data_file, position, SEEK_SET);

  		getfieldarray();
  		fields_counter();
  		printf("\n-----------------------------Details--------------------------\n");
  		while(counter < field_count)
    	{
			if(fread(field_Data, sizeof(char), LENGTH, fp_data_file))
			{   
	        
				printf("%s : %s\n", field_array[counter], field_Data);
			    counter++;
        	}

   		}
    }

    else
    {

    }
    fclose(fp_data_file);
}


void update()
{
	int position = get_record_postion();
	char field_Data[LENGTH];
	int user_choice;
  if(position != -1)
   {
  		 int counter = 1;
       fields_counter();
       getfieldarray();
       printf("\nEnter field to update:\n ");
       while(counter < field_count)
       {
       	printf("%d)%s\n", counter, field_array[counter]);
       	counter++;
       }
  		 scanf("%d", &user_choice);
  		 if(user_choice > 0 && user_choice <field_count)
  		 {

		  	 	 fp_data_file = fopen(DATA_FILE, "r+");

		  		 printf("Enter %s to update: ",field_array[user_choice ]);
		  		 scanf(" %[^\n]s", field_Data);
		  		 fseek(fp_data_file, position + (LENGTH * user_choice), SEEK_SET);
		  		 if(fwrite(field_Data, sizeof(char), LENGTH, fp_data_file))
		  		  {
		  				printf("\n%s %s...\n", field_array[user_choice], custom_message[2]);
		  		  }fclose(fp_field_file);
  		 }
    }
    else
    {
    	printf("\nno record found.\n");
    }
}

void delete()
{
	int position = get_record_postion();
	char field_Data[LENGTH];
	if(position != -1)
	{
		fp_data_file = fopen(DATA_FILE,"r+");
		fseek(fp_data_file , position - 1, SEEK_SET);
		fputc('D', fp_data_file);
		fclose(fp_data_file);
		printf("%s %s.", field_array[0],custom_message[3]);
	}
	else
	{
		printf("\nrecord not found\n");
	}

  
}