// Зад. 66 2018-SE-03 Напишете програма на C, която да работи подобно на командата cut, реализирайки само следната функционалност:
// • програмата трябва да чете текст от стандартния вход и да извежда избраната част от всеки ред на стандартния изход;
// • ако първият параметър на програмата е низът -c, тогава вторият параметър е или едноцифрено число (от 1 до 9), или две едноцифрени числа N и M (N ≤ M ), разделени с тире (напр. 3-5). 
// В този случай програмата трябва да изведе избраният/избраните символи от реда: или само символа на указаната позиция, или няколко последователни символи на посочените позиции.
// • ако първият параметър на програмата е низът -d, тогава вторият параметър е низ, от който е важен само първият символ; той се използва като разделител между полета на реда. 
// Третият параметър трябва да бъде низът -f, а четвъртият - или едноцифрено число (от 1 до 9), или две едноцифрени числа N и M (N ≤ M ), разделени с тире (напр. 3-5). 
// В този случай програмата трябва да разглежда реда като съставен от няколко полета (може би празни), разделени от указания символ (първият символ от низа, следващ парметъра -d), 
// и да изведе или само указаното поле, или няколко последователни полета на указаните позиции, разделени от същия разделител.
// • ако някой ред няма достатъчно символи/полета, за него програмата трябва да изведе каквото (докъдето) е възможно (или дори празен ред).

#include <err.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(int argc, char* argv[])
{
    if(argc < 3 || argc > 5 || argc == 4)
    {
		errx(1, "Wrong number of param");
	}

    if (argc == 1)
    {
        char buf[4096];
        ssize_t read_bytes;

        while ((read_bytes = read(0, &buf, sizeof(buf))) > 0)
        {
            if (write(1, &buf, read_bytes) != read_bytes)
            {
                err(1, "Error writing");
            }
        }

        if (read_bytes == -1)
        {
            err(2, "Error reading");
        }

        exit(0);
    }

	if((strcmp(argv[1], "-c")) == 0)
    {
		if(argc != 3)
        {
			errx(2, "Wrong number of parameters for opiton -c");
		}

		if(strchr(argv[2], '-') == NULL)
        {
			char c;
			ssize_t count = 0;
			while(read(0, &c, sizeof(c)) == sizeof(c))
            {
				++count;
				if(count == ((ssize_t)argv[2][0]-(ssize_t)'0'))
                {
					write(1,&c, sizeof(c));
					break;
				}		
			}		
		}
		else
        {
			char c;
			ssize_t count = 0;
			while(read(0, &c, sizeof(c)) == sizeof(c))
            {
				++count;
				if(count >= ((ssize_t)argv[2][0]-(ssize_t)'0') && count <=((ssize_t)argv[2][2]-(ssize_t)'0'))
				{
					write(1, &c, sizeof(c));
				}
			}
		}	
	}
	else if((strcmp(argv[1], "-d")) == 0)
    {
		if(argc != 5)
        {
			errx(3, "Wrong number of parameters for option -d");
		}

		if((strcmp(argv[3], "-f")) != 0)
        {
			err(4, "Invalid third parameter");
		}

		char c;
		ssize_t delimeter = 1;
		if(strchr(argv[4], '-') == NULL)
        {
			while((read(0, &c, sizeof(c))) == sizeof(c))
            {
				if(argv[2][0] == c)
                {
					++delimeter;
				}
				else
                {
					if(delimeter == ((ssize_t)argv[4][0]-(ssize_t)'0'))
                    {
							write(1,&c,sizeof(c));
					}
				}			
			}
		}
		else
        {
			while((read(0,&c,sizeof(c))) == sizeof(c))
            {
				if(argv[2][0] == c)
                {
					++delimeter;
				}
				else
                {
					if(delimeter >= ((ssize_t)argv[4][0]-(ssize_t)'0') && delimeter <= ((ssize_t)argv[4][2]-(ssize_t)'0'))
                    {
						write(1,&c,sizeof(c));
					}
				}
			}
		}
	}
	else
    {
		err(2, "Invalid first parameter");
	}

	printf("\n");
    exit(0);
}