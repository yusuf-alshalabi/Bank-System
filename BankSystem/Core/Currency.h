#pragma once

#include<iostream>
#include<string>
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"
#include <vector>
#include <fstream>
class Currency
{

private:

    enum enMode { EmptyMode = 0, UpdateMode = 1 };
    enMode _Mode;

    std::string _Country;
    std::string _CurrencyCode;
    std::string _CurrencyName;
    double _Rate;

    static Currency _ConvertLinetoCurrencyObject(std::string Line, std::string Seperator = "#//#")
    {
        std::vector<std::string> vCurrencyData;
        vCurrencyData = Core::String::Split(Line, Seperator);

        return Currency(enMode::UpdateMode, vCurrencyData[0], vCurrencyData[1], vCurrencyData[2],
            stod(vCurrencyData[3]));

    }

    static std::string _ConverCurrencyObjectToLine(Currency Currency, std::string Seperator = "#//#")
    {

        std::string stCurrencyRecord = "";
        stCurrencyRecord += Currency.Country() + Seperator;
        stCurrencyRecord += Currency.CurrencyCode() + Seperator;
        stCurrencyRecord += Currency.CurrencyName() + Seperator;
        stCurrencyRecord += std::to_string(Currency.Rate());

        return stCurrencyRecord;

    }

    static  std::vector <Currency> _LoadCurrencysDataFromFile()
    {

        std::vector <Currency> vCurrencys;

        std::fstream MyFile;
        MyFile.open("Currencies.txt", std::ios::in);//read Mode

        if (MyFile.is_open())
        {

            std::string Line;

            while (getline(MyFile, Line))
            {

                Currency Currency = _ConvertLinetoCurrencyObject(Line);

                vCurrencys.push_back(Currency);
            }

            MyFile.close();

        }

        return vCurrencys;

    }

    static void _SaveCurrencyDataToFile(std::vector <Currency> vCurrencys)
    {

        std::fstream MyFile;
        MyFile.open("Currencies.txt", std::ios::out);//overwrite

        std::string DataLine;

        if (MyFile.is_open())
        {

            for (Currency C : vCurrencys)
            {
                DataLine = _ConverCurrencyObjectToLine(C);
                MyFile << DataLine << std::endl;



            }

            MyFile.close();

        }

    }

    void _Update()
    {
        std::vector <Currency> _vCurrencys;
        _vCurrencys = _LoadCurrencysDataFromFile();

        for (Currency& C : _vCurrencys)
        {
            if (C.CurrencyCode() == CurrencyCode())
            {
                C = *this;
                break;
            }

        }

        _SaveCurrencyDataToFile(_vCurrencys);

    }

    static Currency _GetEmptyCurrencyObject()
    {
        return Currency(enMode::EmptyMode, "", "", "", 0);
    }

public:

    Currency(enMode Mode, std::string Country, std::string CurrencyCode, std::string CurrencyName, float Rate)
    {
        _Mode = Mode;
        _Country = Country;
        _CurrencyCode = CurrencyCode;
        _CurrencyName = CurrencyName;
        _Rate = Rate;
    }

    static std::vector <Currency> GetAllUSDRates()
    {

        return _LoadCurrencysDataFromFile();

    }

    bool IsEmpty()
    {
        return (_Mode == enMode::EmptyMode);
    }

    std::string Country()
    {
        return _Country;
    }

    std::string CurrencyCode()
    {
        return _CurrencyCode;
    }

    std::string CurrencyName()
    {
        return _CurrencyName;
    }

    void UpdateRate(float NewRate)
    {
        _Rate = NewRate;
        _Update();
    }

    float Rate()
    {
        return _Rate;
    }


    static Currency FindByCode(std::string CurrencyCode)
    {

        CurrencyCode = Core::String::UpperAllString(CurrencyCode);

        std::fstream MyFile;
        MyFile.open("Currencies.txt", std::ios::in);//read Mode

        if (MyFile.is_open())
        {
            std::string Line;
            while (getline(MyFile, Line))
            {
                Currency Currency = _ConvertLinetoCurrencyObject(Line);
                if (Currency.CurrencyCode() == CurrencyCode)
                {
                    MyFile.close();
                    return Currency;
                }
            }

            MyFile.close();

        }

        return _GetEmptyCurrencyObject();

    }

    static   Currency FindByCountry(std::string Country)
    {
        Country = Core::String::UpperAllString(Country);

        std::fstream MyFile;
        MyFile.open("Currencies.txt", std::ios::in);//read Mode

        if (MyFile.is_open())
        {
            std::string Line;
            while (getline(MyFile, Line))
            {
                Currency Currency = _ConvertLinetoCurrencyObject(Line);
                if (Core::String::UpperAllString(Currency.Country()) == Country)
                {
                    MyFile.close();
                    return Currency;
                }

            }

            MyFile.close();

        }

        return _GetEmptyCurrencyObject();

    }

    static bool IsCurrencyExist(std::string CurrencyCode)
    {
        Currency C1 = Currency::FindByCode(CurrencyCode);
        return (!C1.IsEmpty());

    }

    static std::vector <Currency> GetCurrenciesList()
    {
        return _LoadCurrencysDataFromFile();
    }
};



