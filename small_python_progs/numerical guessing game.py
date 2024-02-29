from random import *
#числовая угадайка, с возможностью начала новой игры и возможностью смены границ в течение программы
###########################################################################################
def is_valid(num_p, m, n):
    if num_p <= m and num_p >= n:
        return True
    else:
        return False

def is_answer(answer):
    if answer == "y" or answer == "n":
        return True
    else:
        return False
###########################################################################################
n = int(input("Введите левую границу поиска(включительно): "))
m = int(input("Введите левую границу поиска(включительно): "))
###########################################################################################
num = randint(n, m)
answer = False
correct_answer_newgame = False
correct_answer_borders = False
count = 0
count_try = 0
tries = []
###########################################################################################
replies = ["Ты по-моему перепутал...", "Ответ неверный, введите заново", "Попробуй изменить раслкадку (alt + shift)", 'Введите "y", либо "n", "yes" и "no" соответственно.']
###########################################################################################
print("Ваша задача угадать загаданное число.")
print("Удачи!")
###########################################################################################
while answer == False:
    num_p = int(input("Введите предполагаемое число: "))

    if is_valid(num_p, m, n) == True:

        if num_p > num:
            print("Ваше число больше загаданного, попробуйте снова.")
            count += 1
        elif num_p < num:
            print("Ваше число меньше загаданного, попробуйте снова.")
            count += 1
        else:
            print("Браво! Вы угадали! Было загадано число", num_p)
            count += 1

            while correct_answer_newgame == False:

                new_game = input("Хотите повторить игру? (y/n): ")
                if is_answer(new_game):

                    if new_game == "y":
                        while correct_answer_borders == False:

                            borders = input("Хотите изменить границы поиска? (y/n): ")
                            if is_answer(borders):

                                if borders == "y":
                                    n = int(input("Введите левую границу поиска(включительно): "))
                                    m = int(input("Введите левую границу поиска(включительно): "))
                                    print("Повторим с границами [" + str(n) + ":" + str(m) + "]" + ".")
                                else:
                                    print("Отлично! Повторим с прежними границами!")

                                print("Начинаем заново!")
                                break

                            else:

                                print(replies[randrange(len(replies))])

                        num = randint(n, m)
                        answer == False
                    else:
                        answer = True

                    tries.append(count)
                    count = 0

                    break

                else:

                    print(replies[randrange(len(replies))])

    else:
        print("Число вне диапозона поиска.")
    
###########################################################################################
print("Общее количество попыток:", count)
print("Игра по счёту: количество попыток")
for i in range(1, len(tries) + 1):
    print(str(i) + ":" + " " + str(tries[i - 1]))
print("Game Over!")