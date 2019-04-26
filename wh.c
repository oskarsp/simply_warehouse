#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/******************************************************************************/
#define PROVIDERS_FILE      "providers.bin"
#define GOODS_FILE          "goods.bin"

#define FALSE   -1
#define TRUE     0

#define PRINT_TOP       system("clear");\
                        /*printf("%*s providers_max_id [%d] goods qty [%10.2lf]\n",\
                                        50," ",providers_max_id, goods_qty);*/\
                        printf("Goods: [%10d] \tProviders: [%10d]\n\n",\
                                        goods_cnt, providers_cnt);

#define PRINT_BOTTOM(x,y,z)    printf("\nGoods:\t\tRec [%10d] \n"\
                                      "Balance: \tSum [%15.2lf] \tQty [%10.2lf] \n\n",\
                                       x, y, z);
 
/******************************************************************************/
typedef enum sort_by{
        sort_by_code=1,
        sort_by_name,
        sort_by_unit,
        sort_by_price,
        sort_by_qty,
        sort_by_providers,
        sort_by_address,
        sort_by_city,
        sort_by_country
} sort_by_t;

typedef int bool;

typedef struct {
    char code[20+1];
    char name[64+1];
    char unit[10+1];
    float price;
    float qty;
    int providers_id;
} goods_t;

typedef struct goods_lst_s {
    goods_t goods;
    struct goods_lst_s *next;
} goods_lst_t ;

typedef struct {
    int id;
    char code[10+1];
    char name[50+1];
    char address[50+1];
    char city[30+1];
    char country[30+1];
} providers_t;

typedef struct providers_lst_s {
    providers_t providers;
    struct providers_lst_s *next;
} providers_lst_t ;

/******************************************************************************/
goods_lst_t *goods_lst=NULL;
providers_lst_t *providers_lst=NULL;
int goods_cnt=0;
float goods_qty=0;
int providers_cnt=0;
int providers_max_id=0;

/******************************************************************************/
/* Goods menu */
void menu_goods();
/* Function to read goods from file */
void read_goods();
/* Function to save goods in file*/
void save_goods();
/* Function to add goods to structure/list */
void add_goods(goods_t *p_goods);
/* Function to list goods */
void list_goods(goods_lst_t *p_lst, goods_t *goods);
/* Function to delete from goods */
void delete_goods(goods_t *p_goods);

/* Providers menu */
void menu_providers();
/* Function to read providers from file */
void read_providers();
/* Function to save providers to file */
void save_providers();
/* Function to save goods to file */
void add_providers(providers_t *p_providers, bool isnew);
/* Function to list providers */
void list_providers(providers_lst_t *p_lst, providers_t *providers);
/* Function to delete from providers */
void delete_providers(providers_t *p_providers);

/* Get providers CODE by ID */
char *providers_code_by_id(int providers_id);
/* Get providers ID by CODE*/
int providers_id_by_code(char *providers_code);
/* input data with defined length */
void data_input(char *field, int len);
/* Function to sort providers */
void providers_sort(providers_lst_t **pp_lst, sort_by_t sort_by);
/* Function to sort goods */
void goods_sort(goods_lst_t **pp_lst, sort_by_t sort_by);
/* Check for exist goods code */
int goods_by_code(char *goods_code);
/* Function to delete goods from list by code */
void goods_delete(goods_lst_t **pp_lst, char *code);
/* Function to delete providers from list by code */
void providers_delete(providers_lst_t **pp_lst, char *code);
/******************************************************************************/
int main ()
{
    int choice;
    char input[10];

    read_goods(&goods_lst);
    read_providers(&providers_lst);
    
    do {
        PRINT_TOP;

        printf("=== Main menu ===\n\n");

        printf("1 - Goods\n");
        printf("2 - Providers\n");
        printf("3 - Exit\n");

        printf("\nPlease Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice=atoi(input);

            switch (choice)
        {
            case 1:
                menu_goods();
                break;
            case 2:
                menu_providers();
                break;
            case 3:
                return 0;
        }
    } while (choice!=3);
}

/******************************************************************************/
void menu_goods()
{
    int choice;
    char input[10];
    goods_t goods;
    int fld_len=0;
    char price[10+1];
    char qty[10+1];
    char providers_code[10+1];

    do {
        PRINT_TOP;

        printf("=== Goods menu ===\n\n");

        printf("1 - List goods\n");
        printf("2 - Add goods\n");
        printf("3 - Delete goods (by Code)\n");
        printf("4 - Search/Filter goods\n");
        printf("5 - Return\n");

        printf("\nPlease Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice=atoi(input);

        switch (choice)
        {
            case 1:
                system("clear");
                list_goods(goods_lst, NULL);
                break;
            case 2:
                memset(&goods, 0, sizeof(goods_t));

                fld_len=(int)sizeof(goods.code)-1;
                printf("Goods    Code[%d]: ", fld_len );
                data_input(goods.code, fld_len);

                /* check for duplicate*/
                if ( TRUE == goods_by_code(goods.code) )
                {
                    printf("!!! Duplicate Code !!!");
                    printf("Press [Enter] key to continue.\n");
                    while(getchar()!='\n'); // clean stdin
                }
                else
                {
                    fld_len=(int)sizeof(goods.name)-1;
                    printf("Goods    Name[%d]: ", fld_len);
                    data_input(goods.name, fld_len);

                    fld_len=(int)sizeof(goods.unit)-1;
                    printf("Goods    Unit[%d]: ", fld_len);
                    data_input(goods.unit, fld_len);

                    fld_len=10;
                    printf("Goods    price[%d]: ", fld_len);
                    data_input(price, fld_len);
                    goods.price=atof(price);

                    fld_len=10;
                    printf("Goods    qty[%d]: ", fld_len);
                    data_input(qty, fld_len);
                    goods.qty=atof(qty);

                    fld_len=10;
                    printf("Goods provider code[%d]: ", fld_len);
                    data_input(providers_code, fld_len);

                    if ( 0>(goods.providers_id=providers_id_by_code(providers_code)) )
                    {
                        printf("!!! Wrong Provider Code !!!.Not saved\n");
                    }
                    else
                    {
                        /* Add to goods list */
                        add_goods(&goods);
                        /* Save pgoods to file */
                        save_goods();
                    }

                    printf("Press [Enter] key to continue.\n");
                    while(getchar()!='\n'); // clean stdin
                }
                break;
            case 3:
                memset(&goods, 0, sizeof(goods_t));

                fld_len=(int)sizeof(goods.code)-1;
                printf("\n\nPlease enter goods code[%d] to delete: ", fld_len );
                data_input(goods.code, fld_len);

                /* Delete goods by code */
                goods_delete(&goods_lst, goods.code);

                /* Save pgoods to file */
                save_goods();

                break;
            case 4:
                memset(&goods, 0, sizeof(goods_t));
                
                printf("\n\nPlease enter data for filtering Goods by Code/Name/Providers Code\n\n");

                fld_len=(int)sizeof(goods.code)-1;
                printf("Goods    Code[%d]: ", fld_len );
                data_input(goods.code, fld_len);

                fld_len=(int)sizeof(goods.name)-1;
                printf("Goods    Name[%d]: ", fld_len);
                data_input(goods.name, fld_len);

                fld_len=10;
                printf("Goods provider code[%d]: ", fld_len);
                data_input(providers_code, fld_len);
                goods.providers_id=providers_id_by_code(providers_code);

                list_goods(goods_lst, &goods);
                break;
            case 5:
                return;
                break;
        }
    } while (choice!=5);
}

/******************************************************************************/
void menu_providers()
{
    int choice;
    char input[10];
    providers_t providers;
    int fld_len=0;

    do {
        PRINT_TOP;

        printf("=== Providers menu ===\n\n");

        printf("1 - List Providers\n");
        printf("2 - Add Providers\n");
        printf("3 - Delete Providers (by Code)\n");
        printf("4 - Search/Filter providers\n");
        printf("5 - Return\n");

        printf("\nPlease Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice=atoi(input);

        switch (choice)
        {
            case 1:
                list_providers(providers_lst, NULL);
                break;
            case 2:
                memset(&providers, 0, sizeof(providers_t));

                fld_len=(int)sizeof(providers.code)-1;
                printf("Provider    Code[%d]: ", fld_len );
                data_input(providers.code, fld_len);

                /* check for duplicate*/
                if ( 0 <= providers_id_by_code(providers.code) )
                {
                    printf("!!! Duplicate Code !!!");
                    printf("Press [Enter] key to continue.\n");
                    while(getchar()!='\n'); // clean stdin
                }
                else
                {
                    fld_len=(int)sizeof(providers.name)-1;
                    printf("Provider    Name[%d]: ", fld_len);
                    data_input(providers.name, fld_len);

                    fld_len=(int)sizeof(providers.address)-1;
                    printf("Provider Address[%d]: ", fld_len);
                    data_input(providers.address, fld_len);

                    fld_len=(int)sizeof(providers.city)-1;
                    printf("Provider    City[%d]: ", fld_len);
                    data_input(providers.city, fld_len);

                    fld_len=(int)sizeof(providers.country)-1;
                    printf("Provider Country[%d]: ", fld_len);
                    data_input(providers.country, fld_len);

                    /* Add to providers list */
                    add_providers(&providers, TRUE);
                    /* Save providers to file */
                    save_providers();

                    printf("Press [Enter] key to continue.\n");
                    while(getchar()!='\n'); // clean stdin
                }
                break;
            case 3:
                memset(&providers, 0, sizeof(providers_t));

                fld_len=(int)sizeof(providers.code)-1;
                printf("\n\nPlease enter Provider code[%d] to delete: ", fld_len );
                data_input(providers.code, fld_len);

                /* Dolete providers by code */
                providers_delete(&providers_lst, providers.code);

                /* Save providers to file */
                save_providers();
                break;
            case 4:
                /* TODO filtrēt pēc kods, nosaukums, valsts - piegadatajs */
                memset(&providers, 0, sizeof(providers_t));
                
                printf("\n\nPlease enter data for filtering Providers by Code/Name/Country\n\n");

                fld_len=(int)sizeof(providers.code)-1;
                printf("Provider    Code[%d]: ", fld_len );
                data_input(providers.code, fld_len);

                fld_len=(int)sizeof(providers.name)-1;
                printf("Provider    Name[%d]: ", fld_len);
                data_input(providers.name, fld_len);

                fld_len=(int)sizeof(providers.country)-1;
                printf("Provider Country[%d]: ", fld_len);
                data_input(providers.country, fld_len);

                list_providers(providers_lst, &providers);
                break;
            case 5:
                return;
                break;
        }
    } while (choice!=5);
    
}

/******************************************************************************/
void read_goods()
{
    goods_t *goods=malloc(sizeof(goods_t));
    FILE *fp=NULL;
    
    fp = fopen("goods.bin", "rb+");
    if (NULL == fp)
    {
        fp = fopen("goods.bin", "wb+");
    }

    goods_cnt=0 ;
    goods_qty=0.0;
    while( fread(goods, sizeof(goods_t), 1, fp) > 0 ) 
    {
        add_goods(goods);
        goods_qty = goods_qty + (float)goods->qty;
    }
    
    fclose(fp);
}

/******************************************************************************/
void add_goods(goods_t *p_goods) 
{
    goods_lst_t *tmp = NULL;
    
    if (NULL == goods_lst)
    {
        tmp = malloc(sizeof(goods_lst_t));
        tmp->goods=*p_goods;
        tmp->next=NULL;
        goods_lst=tmp;
    }
    else
    {
        tmp = goods_lst;
        while (NULL != tmp->next)
        {
            tmp = tmp->next;
        }

        tmp->next = malloc(sizeof(goods_lst_t));
        tmp->next->goods=*p_goods;
        tmp->next->next=NULL;
    }

    goods_cnt++ ;
}

/******************************************************************************/
void read_providers()
{
    providers_t *providers=malloc(sizeof(providers_t));
    FILE *fp=NULL;
    char *fname="providers.bin";
    
    fp = fopen(fname, "rb+");
    if (NULL == fp)
    {
        fp = fopen(fname, "wb+");
    }

    providers_cnt=0 ;
    while( fread(providers, sizeof(providers_t), 1, fp) > 0 ) 
    {
        if (providers_max_id < providers->id ) 
        {
            providers_max_id=providers->id;
        }
        add_providers(providers, FALSE);
    }
    fclose(fp);
}

/******************************************************************************/
void add_providers(providers_t *p_providers, bool isnew) 
{
    providers_lst_t *tmp = NULL;
    
    if (TRUE == isnew)
    {
        providers_max_id++;
        p_providers->id=providers_max_id;
    }

    if (NULL == providers_lst)
    {
        tmp = malloc(sizeof(providers_lst_t));
        tmp->providers=*p_providers;
        tmp->next=NULL;
        providers_lst=tmp;
    }
    else
    {
        tmp = providers_lst;
        while (NULL != tmp->next)
        {
            tmp = tmp->next;
        }

        tmp->next = malloc(sizeof(providers_lst_t));
        tmp->next->providers=*p_providers;
        tmp->next->next=NULL;
    }

    providers_cnt++ ;
}

/******************************************************************************/
void save_providers()
{
    providers_lst_t *tmp=providers_lst;
    int cnt=0 ;
    FILE *fp=NULL;

    fp = fopen(PROVIDERS_FILE, "wb");

    if ( NULL==fp || NULL==providers_lst ) {
        printf("filed to save.\n") ;
        return ;
    }

    while (tmp != NULL ) {
        cnt++ ;
        fwrite(&tmp->providers, sizeof(providers_t), 1, fp) ;
        tmp = tmp->next ;
    }
    fclose(fp);
}

/******************************************************************************/
void list_providers(providers_lst_t *p_lst, providers_t *providers) 
{
    providers_lst_t *tmp = p_lst;
    int choice;
    char input[10];
    bool skip_print=FALSE;


    PRINT_TOP;

    if (0 == providers_cnt)
    {
        printf("No providers to list!!\n\n");
    }
    else
    {
        printf("1. Sort by Code\n");
        printf("2. Sort by Name\n");
        printf("3. Sort by Address\n");
        printf("4. Sort by City\n");
        printf("5. Sort by Country\n");

        printf("\nPlease Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice=atoi(input);

        switch (choice)
        {
            case 1:
                providers_sort(&tmp, sort_by_code);
                break;
            case 2:
                providers_sort(&tmp, sort_by_name);
                break;
            case 3:
                providers_sort(&tmp, sort_by_address);
                break;
            case 4:
                providers_sort(&tmp, sort_by_city);
                break;
            case 5:
                providers_sort(&tmp, sort_by_country);
                break;
            default:
                providers_sort(&tmp, -1);
        }

        PRINT_TOP;
        printf("+=========================================================================================================================================================================================+\n");
        printf("|%10s|%50s|%50s|%30s|%30s|%10s|\n",
               "CODE", "NAME","ADDRESS","CITY","COUNTRY","ID");
        printf("+=========================================================================================================================================================================================+\n");
        while (NULL != tmp)
        {
            skip_print=FALSE;
            if ( NULL != providers )
                if ( 0<strlen(providers->code) && NULL==strstr(tmp->providers.code, providers->code ) ||
                     0<strlen(providers->name) && NULL==strstr(tmp->providers.name, providers->name) ||
                     0<strlen(providers->country) && NULL==strstr(tmp->providers.country, providers->country) )
            {
                skip_print=TRUE;
            }

            if (TRUE != skip_print)
            {
                printf("|%10s|%50s|%50s|%30s|%30s|%10d|\n",
                    tmp->providers.code, 
                    tmp->providers.name, 
                    tmp->providers.address,
                    tmp->providers.city, 
                    tmp->providers.country, 
                    tmp->providers.id);
            }
            tmp = tmp->next;
        }
        printf("+=========================================================================================================================================================================================+\n");
    }

    printf("\n\nPress [Enter] key to continue.\n");
    while(getchar()!='\n'); // to clean stdin
}

/******************************************************************************/
void data_input(char *field, int str_len)
{
    char input_string[512];
    memset(input_string, 0, sizeof(input_string));

    scanf("%[^\n]s",input_string);
    while(getchar()!='\n'); // to clean stdin
    sprintf(field, "%.*s", str_len, input_string);
}

/******************************************************************************/
void list_goods(goods_lst_t *p_lst, goods_t *goods) 
{
    goods_lst_t *tmp = p_lst;
    int choice;
    char input[10];
    int cnt=0;
    float sum_tot=0;
    float sum_qty=0.0f;
    bool skip_print=FALSE;

    PRINT_TOP;

    if (0 == goods_cnt)
    {
        printf("No goods to list!!\n\n");
    }
    else
    {
        printf("1. Sort by Code\n");
        printf("2. Sort by Name\n");
        printf("3. Sort by Price\n");
        printf("4. Sort by Qty\n");
        printf("5. Sort by Providers\n");

        printf("\nPlease Enter your choice: ");
        fgets(input, sizeof(input), stdin);
        choice=atoi(input);

        switch (choice)
        {
            case 1:
                goods_sort(&tmp, sort_by_code);
                break;
            case 2:
                goods_sort(&tmp, sort_by_name);
                break;
            case 3:
                goods_sort(&tmp, sort_by_price);
                break;
            case 4:
                goods_sort(&tmp, sort_by_qty);
                break;
            case 5:
                goods_sort(&tmp, sort_by_providers);
                break;
            default:
                goods_sort(&tmp, -1);
        }

        PRINT_TOP;
        printf("+======================================================================================================================================+\n");
        printf("|%20s|%64s|%10s|%15s|%10s|%10s|\n",
               "CODE", "GOODS NAME","UNIT","PRICE","QTY","PROV.CODE");
        printf("+======================================================================================================================================+\n");
        while (NULL != tmp)
        {
            skip_print=FALSE;
            if ( NULL != goods )
            {
                if ( 0<strlen(goods->code) && NULL==strstr(tmp->goods.code, goods->code ) ||
                     0<strlen(goods->name) && NULL==strstr(tmp->goods.name, goods->name) ||
                     goods->providers_id!=-1 && tmp->goods.providers_id != goods->providers_id )
                {
                    skip_print=TRUE;
                }
            }

            if (TRUE != skip_print)
            {
                printf("|%20s|%64s|%10s|%15.2lf|%10.2lf|%10s|\n",
                    tmp->goods.code, 
                    tmp->goods.name, 
                    tmp->goods.unit,
                    tmp->goods.price,
                    tmp->goods.qty,
                    providers_code_by_id(tmp->goods.providers_id));

                cnt++;
                sum_qty += tmp->goods.qty;
                sum_tot += (tmp->goods.qty * tmp->goods.price);
            }

            tmp = tmp->next;
        }
        printf("+======================================================================================================================================+\n");
    }

    PRINT_BOTTOM(cnt, sum_tot, sum_qty);

    printf("\n\nPress [Enter] key to continue.\n");
    while(getchar()!='\n'); // to clean stdin
}

/******************************************************************************/
void save_goods()
{
    goods_lst_t *tmp=goods_lst;
    int cnt=0 ;
    FILE *fp=NULL;

    fp = fopen(GOODS_FILE, "wb");

    if ( NULL==fp || NULL==goods_lst ) {
        printf("filed to save.\n") ;
        return ;
    }

    while (tmp != NULL ) {
        cnt++ ;
        fwrite(&tmp->goods, sizeof(goods_t), 1, fp) ;
        tmp = tmp->next ;
    }
    fclose(fp);
}

/******************************************************************************/
char *providers_code_by_id(int providers_id)
{
    providers_lst_t *tmp = providers_lst;
    while (NULL != tmp)
    {
        if (providers_id == tmp->providers.id)
        {
            return tmp->providers.code;
        }
        tmp = tmp->next;
    }
}

/******************************************************************************/
int providers_id_by_code(char *providers_code)
{
    providers_lst_t *tmp = providers_lst;
    int providers_id=-1;
    while (NULL != tmp)
    {
        if (0==strcmp(providers_code, tmp->providers.code))
        {
            providers_id = tmp->providers.id;
            break;
        }
        tmp = tmp->next;
    }
    return providers_id;
}

/******************************************************************************/
int goods_by_code(char *goods_code)
{
    goods_lst_t *tmp = goods_lst;

    while (NULL != tmp)
    {
        if (0==strcmp(goods_code, tmp->goods.code))
        {
            return TRUE;
        }
        tmp = tmp->next;
    }
    return FALSE;
}

/******************************************************************************/
void providers_sort(providers_lst_t **pp_lst, sort_by_t sort_by)
{
    int i, j, k;
    providers_lst_t *current;
    providers_lst_t *next;
    providers_t *tmp_providers;
    int size=0;
    char val1[512], val2[512];

    for ( current = *pp_lst; current != NULL; current = current->next ) 
    {
        size++;
    }

    k = size;

    for ( i = 0; i < size-1; i++, k--)
    {
        current = *pp_lst;
        next = (*pp_lst)->next;

        for ( j = 1; j < k; j++ )
        {
            switch (sort_by)
            {
                case sort_by_code:
                    sprintf(val1,"%10s", current->providers.code);
                    sprintf(val2,"%10s", next->providers.code);
                    break;
                case sort_by_name:
                    sprintf(val1,"%50s", current->providers.name);
                    sprintf(val2,"%50s", next->providers.name);
                    break;
                case sort_by_address:
                    sprintf(val1,"%50s", current->providers.address);
                    sprintf(val2,"%50s", next->providers.address);
                    break;
                case sort_by_city:
                    sprintf(val1,"%30s", current->providers.city);
                    sprintf(val2,"%30s", next->providers.city);
                    break;
                case sort_by_country:
                    sprintf(val1,"%30s", current->providers.country);
                    sprintf(val2,"%30s", next->providers.country);
                    break;
                default:
                    sprintf(val1,"%10d", current->providers.id);
                    sprintf(val2,"%10d", next->providers.id);
            }

            if ( 0<strcmp(val1, val2) )
            {
                tmp_providers=malloc(sizeof(providers_lst_t));
                *tmp_providers=current->providers;
                current->providers=next->providers;
                next->providers=*tmp_providers;
            }
            current = current->next;
            next = next->next;
        }
    }
}

/******************************************************************************/
void goods_sort(goods_lst_t **pp_lst, sort_by_t sort_by)
{
    int i, j, k;
    goods_lst_t *current;
    goods_lst_t *next;
    goods_t *tmp_goods;
    int size=0;
    char val1[128], val2[128];

    for ( current = *pp_lst; current != NULL; current = current->next ) 
    {
        size++;
    }

    k = size;

    for ( i = 0; i < size-1; i++, k--)
    {
        current = *pp_lst;
        next = (*pp_lst)->next;

        for ( j = 1; j < k; j++ )
        {
            switch (sort_by)
            {
                case sort_by_code:
                    sprintf(val1,"%20s", current->goods.code);
                    sprintf(val2,"%20s", next->goods.code);
                    break;
                case sort_by_name:
                    sprintf(val1,"%65s", current->goods.name);
                    sprintf(val2,"%65s", next->goods.name);
                    break;
                case sort_by_price:
                    sprintf(val1,"%15.2lf", current->goods.price);
                    sprintf(val2,"%15.2lf", next->goods.price);
                    break;
                case sort_by_qty:
                    sprintf(val1,"%10.2lf", current->goods.qty);
                    sprintf(val2,"%10.2lf", next->goods.qty);
                    break;
                case sort_by_providers:
                    sprintf(val1,"%10s", providers_code_by_id(current->goods.providers_id));
                    sprintf(val2,"%10s", providers_code_by_id(next->goods.providers_id));
                    break;

                default:
                    sprintf(val1,"%20s", current->goods.code);
                    sprintf(val2,"%20s", next->goods.code);
            }

            if ( 0<strcmp(val1, val2) )
            {
                tmp_goods=malloc(sizeof(goods_lst_t));
                *tmp_goods=current->goods;
                current->goods=next->goods;
                next->goods=*tmp_goods;
            }
            current = current->next;
            next = next->next;
        }
    }
}

/******************************************************************************/
void goods_delete(goods_lst_t **pp_lst, char *code)
{
    goods_lst_t *tmp = *pp_lst;
    goods_lst_t *prev = NULL;
    goods_lst_t *next = NULL;
    int i =0;

   while (NULL != tmp)
   {
        next = tmp->next;
        if ( 0==strcmp(tmp->goods.code, code) ) 
        {
            goods_cnt--;
            i++;
            free(tmp);
            if (NULL != prev)
            {
                prev->next = next;
            }
            else
            {
                *pp_lst = next;
            }
        }
        else
        {
            prev = tmp;
        }
        tmp = next;
    }

    if ( 0<i )
    {
        printf("\nGoods Code=[%20s] Deleted!!!\n\n", code);
    }   
    else
    {
        printf("\nGoods with Code=[%20s] not found!!!\n"
            "Nothing to delete!!!\n\n", code);
    } 

    printf("Press [Enter] key to continue.\n");
    while(getchar()!='\n'); // clean stdin

}

/******************************************************************************/
void providers_delete(providers_lst_t **pp_lst, char *code)
{
    providers_lst_t *tmp = *pp_lst;
    providers_lst_t *prev = NULL;
    providers_lst_t *next = NULL;
    int i = 0;

   while (NULL != tmp)
   {
        next = tmp->next;
        if ( 0==strcmp(tmp->providers.code, code) ) 
        {
            goods_cnt--;
            i++;
            free(tmp);
            if (NULL != prev)
            {
                prev->next = next;
            }
            else
            {
                *pp_lst = next;
            }
        }
        else
        {
            prev = tmp;
        }
        tmp = next;
    }

    if ( 0<i )
    {
        printf("\nProviders Code=[%10s] Deleted!!!\n\n", code);
    }   
    else
    {
        printf("\nProviders with Code=[%10s] not found!!!\n"
            "Nothing to delete!!!\n\n", code);
    }

    printf("Press [Enter] key to continue.\n");
    while(getchar()!='\n'); // clean stdin

}
