import asyncio

from serial import Serial

from aiogram import Router, F, Bot, Dispatcher

from aiogram.filters import CommandStart, Command
from aiogram.filters.callback_data import CallbackData

from aiogram.types import (
    InlineKeyboardMarkup, InlineKeyboardButton,
    FSInputFile
)


Serial = Serial(port="COM18", baudrate=9600)


CLIENT = "client"
MANAGER = "manager"


router = Router()


class Info(CallbackData, prefix="info"):
    state: int = 0
    role: str = '@'
    item: str = '@'
    dest: str = '@'


@router.callback_query(Info.filter(F.state == 4))
@router.callback_query(Info.filter(F.state == 0))
@router.message(CommandStart())
async def start(message):
    if hasattr(message, "message"):
        data = Info.unpack(message.data)
        message = message.message
    else:
        data = None

    layout = [
        [
            InlineKeyboardButton(text="Клиент", callback_data=Info(state=2, role=CLIENT, dest='o').pack()),
            InlineKeyboardButton(text="Менеджер", callback_data=Info(state=1, role=MANAGER).pack())
        ]
    ]
    keyboard = InlineKeyboardMarkup(inline_keyboard=layout)

    if (not data) or (data.state == 4):
        await message.answer_photo(FSInputFile("./image.jpg"), caption="Кем вы являетесь?", reply_markup=keyboard)
    else:
        await message.edit_caption(caption="Кем вы являетесь?", reply_markup=keyboard)


@router.callback_query(Info.filter(F.state == 2))
async def client(callback):
    data = dict(Info.unpack(callback.data))
    layout = [
        [
            InlineKeyboardButton(text="Товар A", callback_data=Info(**{
                **data,
                "state": 3,
                "item": 'A',
            }).pack()),
            InlineKeyboardButton(text="Товар B", callback_data=Info(**{
                **data,
                "state": 3,
                "item": 'B',
            }).pack()),
        ],
        [
            InlineKeyboardButton(text="Назад", callback_data=Info(**{
                **data,
                "state": 0 if data["role"] == CLIENT else 1
            }).pack())
        ]
    ]
    keyboard = InlineKeyboardMarkup(inline_keyboard=layout)
    await callback.message.edit_caption(caption="Выберите товар", reply_markup=keyboard)


@router.callback_query(Info.filter(F.state == 1))
async def manager(callback):
    data = dict(Info.unpack(callback.data))
    layout = [
        [
            InlineKeyboardButton(text="Загрузить", callback_data=Info(**{
                **data,
                "state": 2,
                "dest": 'i'
            }).pack()),
            InlineKeyboardButton(text="Выгрузить", callback_data=Info(**{
                **data,
                "state": 2,
                "dest": 'o'
            }).pack()),
        ],
        [
            InlineKeyboardButton(text="Назад", callback_data=Info(**{
                **data,
                "state": 0
            }).pack())
        ]
    ]
    keyboard = InlineKeyboardMarkup(inline_keyboard=layout)
    await callback.message.edit_caption(caption="Выберите действие", reply_markup=keyboard)


@router.callback_query(Info.filter(F.state == 3))
async def process(callback):
    data = Info.unpack(callback.data)
    layout = [[InlineKeyboardButton(text="На главную", callback_data=Info(state=4).pack())]]
    keyboard = InlineKeyboardMarkup(inline_keyboard=layout)

    a = bytes([0 if data.dest == 'i' else 1])
    b = bytes([0 if data.item == 'A' else 1])

    Serial.write(a)
    Serial.write(b)

    print(a, b)
    
    await callback.message.edit_caption(caption="Выполняем...", reply_markup=keyboard)


async def main():
    bot = Bot(token="<token>")
    dp = Dispatcher()

    dp.include_router(router)
    await dp.start_polling(bot)


if __name__ == "__main__":
    print("Запуск...")
    asyncio.run(main())
