import os
import time 
import zipfile


# интерфейс программы
print("#################################################################################")
print("Вас приветствует консольная программа-архиватор!")
print('Для завершения сбора путей напишите "Enough" без кавычек.')
print('Приятного пользования!')
print("#################################################################################")
print()

# получение имени папки(время создания):
time_s = time.time()  # получение времени с начала эпохи в секундах
time_y = time.ctime(time_s)  # преобразование секунд в нормальное время

# замена недопустимых символов
time_y = time_y.replace(" ", "_") 
time_y = time_y.replace(":", ".") 

# создание папки
directory = os.getcwd()  # получаем местоположение программы
dir_res = os.path.join(os.path.abspath(directory), time_y + ".zip")  # создание правильного пути к папке с программой

arch = zipfile.ZipFile(dir_res, "w", compression=zipfile.ZIP_DEFLATED, allowZip64=True)  # создание архива

# сбор имён требуемых для архивации файлов

i = True

dir_list = []  # объявление списка для путей архивирования

while i:  # сбор списка путей
	dir_el = input("Введите директорию к копированию: ")

	if os.path.isdir(dir_el):
		if dir_el not in dir_list:  # проверка, нет ли в списке этой папки
			dir_list.append(dir_el)

		else: 
			print("Эта папка уже есть в архиве!")

	elif dir_el == "Enough":   # конец сбора
		print("Файлы успешно скопированы!")
		break

	else:   # сообщение в случае неправильного ввода файла или директории
		print("Такой директории не существует")
		continue

for element in dir_list:
	for root, dirs, files in os.walk(element):
		for file in files:
			arch.write(os.path.join(root, file))


arch.close()

close = str(input("Нажмите любую кнопку для завершения..."))  # ввод для закрытия программы
