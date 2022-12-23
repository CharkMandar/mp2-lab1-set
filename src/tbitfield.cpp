// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле

#include "tbitfield.h"


TBitField::TBitField(int len)
{
    if (len < 0) 
        throw "Invalid Value";
    
    this->BitLen = len;
    this->MemLen = len / (sizeof(TELEM) * 8) + 1;
    this->pMem = new TELEM[MemLen];
    for (size_t i = 0; i < this->MemLen; i++)
        this->pMem[i] = 0;

}

TBitField::TBitField(const TBitField &bf) // конструктор копирования
{

    this->BitLen = bf.BitLen;
    this->MemLen = bf.MemLen;
    for (size_t i = 0; i < this->MemLen; i++) {
        this->pMem[i] = bf.pMem[i];
    }

}

TBitField::~TBitField()
{
    delete[] this->pMem;

}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
    if (n < 0 || n >= BitLen)
        throw "Index is out of range";
    return (n / (sizeof(TELEM) * 8));
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{

    if (n > BitLen || n < 0)
        throw "Index is out of range";
    return 1 << n % (sizeof(TELEM) * 8);
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
  return this->BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
    if ((n >= GetLength()))
        throw "Index is out of range";

    this->pMem[GetMemIndex(n)] |= GetMemMask(n);
}

void TBitField::ClrBit(const int n) // очистить бит
{
    if (n >= GetLength())
        throw "Index is out of range";

    this->pMem[GetMemIndex(n)] &= ~GetMemMask(n);
}

int TBitField::GetBit(const int n) const // получить значение бита
{
    if ((n >= this->GetLength())||(n < 0))
        throw "Index is out of range";

  return (this->pMem[GetMemIndex(n)] & GetMemMask(n)) >> (n % (sizeof(TELEM) * 8));
}

// битовые операции

TBitField& TBitField::operator=(const TBitField &bf) // присваивание
{
    if (bf != *this) {
        delete[] this->pMem;
        this->pMem = new TELEM[bf.MemLen];
        this->MemLen = bf.MemLen;
        this->BitLen = bf.BitLen;
        for (int i = 0; i < this->GetLength(); i++) {
            this->pMem[i] = bf.pMem[i];
        }
        return *this;
    }
}

int TBitField::operator==(const TBitField &bf) const // сравнение
{
    if (bf.BitLen != this->BitLen) 
        return 0;

    for (size_t i = 0; i < bf.MemLen; i++) {
        if (this->pMem[i] != bf.pMem[i]) {
            return 0;
        }
    }
    
    return 1;
}

int TBitField::operator!=(const TBitField &bf) const // сравнение
{
  return !(*this == bf);
}

TBitField TBitField::operator|(const TBitField& bf) // операция "или"
{
    TBitField tmp(max(this->MemLen, bf.MemLen));
    for (size_t i = 0; i < tmp.GetLength(); i++)
        tmp.pMem[i] |= bf.pMem[i];
    
    return tmp;

}

TBitField TBitField::operator&(const TBitField &bf) // операция "и"
{
    TBitField tmp(max(this->BitLen, bf.BitLen));
    for (size_t i = 0; i < min(this->BitLen, bf.BitLen); i++)
        tmp.pMem[i] &= bf.pMem[i];
    
    return tmp;
}

TBitField TBitField::operator~(void) // отрицание
{
    TBitField tmp(GetLength());
    for(size_t i = 0; i < this->MemLen; i++)
        tmp.pMem[i] &= this->pMem[i];

    return tmp;
}

// ввод/вывод

istream &operator>>(istream &istr, TBitField &bf) // ввод
{
    int tmp = 0;

    for (size_t i = 0; i < bf.GetLength(); i++) {
        istr >> tmp;
        if (tmp == 1)
            bf.SetBit(i);
        else
            bf.ClrBit(i);

    }
    return istr;
}

ostream &operator<<(ostream &ostr, const TBitField &bf) // вывод
{   
    for (size_t i = 0; i < bf.GetLength(); i++)
        ostr << bf.GetBit(i);
    return ostr;
}
