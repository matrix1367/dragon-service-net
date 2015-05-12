#include "DataBaseSQLITE.h"
#include <cstdio>

CDataBaseSQLITE::CDataBaseSQLITE(std::string namefile) :m_namefile(namefile)
{
    //ctor
}

CDataBaseSQLITE::~CDataBaseSQLITE()
{
    //dtor
}

bool CDataBaseSQLITE::Open()
{
    if(!sqlite3_open(m_namefile.c_str(), &db) ){
        return true;
   }else{
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
      return false;
   }
}

bool CDataBaseSQLITE::Close()
{
    return false;
}


bool CDataBaseSQLITE::Query(std::vector<std::vector<std::string> >& results, const std::string& message)
{
    sqlite3_stmt *statement;

	int res = sqlite3_prepare(db, message.c_str(),  - 1, &statement, 0);
	if( res == SQLITE_OK)
	{
		int cols = sqlite3_column_count(statement);
		int result = 0;
		while(true)
		{
			result = sqlite3_step(statement);

			if(result == SQLITE_ROW)
			{
				std::vector<std::string> values;
				for(int col = 0; col < cols; col++)
				{
				    std::string value="";
				    char* ptr = (char*)sqlite3_column_text(statement, col);
				    if (ptr)
                    {
                        value = std::string(ptr);
                    }

					values.push_back(value);
					//printf("add vector: %s\n",value.c_str());
				}
				results.push_back(values);
			}
			else if (result == SQLITE_DONE) {
                break;
            } else if (result == SQLITE_ERROR)
			{
			    printf("failed!\n");
				break;
			} else
			{
                printf("ERROR : %d!\n", result);
			    break;
			}
		}

		sqlite3_finalize(statement);
	} else {
        printf("Error sql %d\n", res);
	}

	std::string error = sqlite3_errmsg(db);
	if(error != "not an error")
    {
        printf("[SQL]:%s, [ERROR]:%s\n",  message.c_str(), error.c_str());
        return false;
	}
	else
    {
        printf("[SQL]:%s\n", message.c_str());
        return true;
	}
}
