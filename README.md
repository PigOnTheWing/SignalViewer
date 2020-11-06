# Описание
**Signal** -- клиент-серверное приложение для мониторинга непрерывного сигнала.

Приложение представлено 3 проектами:

* **SignalServer** -- серверная часть
* **SignalClient** -- клиентская часть
* **JsonParser** -- динамическая библиотека, используемая в SignalServer и SignalClient

Для обеспечения независимости клиентов, сервер выделяет каждому новому подключению отдельный поток, в котором впоследствии и происходит все взаимодействие

Клиент и сервер общаются посредством json, формат сообщений предельно прост:

```
{ 
	"Command": JsonParser::Command,
	"Attr": param 
}
```

Команды:

* JsonParser::READY -- сервер посылает эту поманду клиенту, чтобы оповестить его о своей готовности
* JsonParser::START -- команда начала генерации
* JsonParser::STOP -- команда паузы генерации
* JsonParser::DISCONNECT -- клиент оповещает сервер об отключении
* JsonParser::NEW_VAL -- команда от сервера, содержащая параметром новое сгенерированное значение
* JsonParser::AMPLITUDE -- команда от клиента, содержащая новое значение амплитуды
* JsonParser::PERIOD -- комагда от клиента, содержащая новое значение периода

# Запуск приложения
После импорта проекта в Qt Creator и компиляции исходников, запуск приложения может осуществляться через:

*  Qt Creator
*  Терминал


### Запуск через терминал
Чтобы запустить проект(ы) через терминал, необходимо экспортировать переменную окружения `LD_LIBRARY_PATH` и указать в ней директорию файла `libJsonParser.so`

``` 
export LD_LIBRARY_PATH=/path/to/lib/
```

# Работа с приложением
Для работы с приложением необходимо запустить **SignalServer** и **SignalClient**.

### Сервер
После запуска сервера, в окно терминала выведется сообщения с **хостом** и **портом**, к которым нужно подключатся клиентам.

### Клиент
Для поключения нужно нажать кнопку **Connect**, после чего появится диалог, в котором нужно указать хост и порт сервера.

После успешного подключения станут доступны кнопки **Disconnect** и **Start**. При нажатии на первую клиент отключится от сервера, нажатие на вторую приводит к началу генерации сигнала.

В центре экрана расположен график сингнала. Слева от графика расположен *слайдер амплитуды*, снизу - *слайдер периода*.
Их изменение приводит к изменению соответствующих величин на сервере.

Также клиент имеет вкладку **Appearance** c двумя пунктами:

* **Width** -- открывает диалог, который позволяет менять ширину сигнальной кривой
* **Colour** -- открывает диалог, который позволяет менять цвет сигнальной кривой
