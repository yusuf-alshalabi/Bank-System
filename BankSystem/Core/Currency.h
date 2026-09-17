#pragma once

#include<iostream>
#include<string>
#include "../../Libs/Cpp-Library-Collection/Lib/String.h"
#include <vector>

class Currency
{

public:

    enum enMode { EmptyMode = 0, UpdateMode = 1 };

    Currency(enMode Mode, std::string Country, std::string CurrencyCode, std::string CurrencyName, float Rate)
    {
        _Mode = Mode;
        _Country = Country;
        _CurrencyCode = CurrencyCode;
        _CurrencyName = CurrencyName;
        _Rate = Rate;
    }

    static std::vector <Currency> GetAllUSDRates();

    bool IsEmpty() const
    {
        return (_Mode == enMode::EmptyMode);
    }

    std::string Country() const
    {
        return _Country;
    }

    std::string CurrencyCode() const
    {
        return _CurrencyCode;
    }

    std::string CurrencyName() const
    {
        return _CurrencyName;
    }

    void UpdateRate(float NewRate)
    {
        _Rate = NewRate;
        _Update();
    }

    float Rate() const
    {
        return static_cast<float>(_Rate);
    }

    static Currency FindByCode(std::string CurrencyCode);

    static   Currency FindByCountry(std::string Country);

    static bool IsCurrencyExist(std::string CurrencyCode);

    static std::vector <Currency> GetCurrenciesList();

    double ConvertToUSD(double Amount) const
    {
        return (double)(Amount / Rate());
    }

    double ConvertToOtherCurrency(double Amount, Currency Currency2) const
    {
        double AmountInUSD = ConvertToUSD(Amount);

        if (Currency2.CurrencyCode() == "USD")
        {
            return AmountInUSD;
        }

        return (double)(AmountInUSD * Currency2.Rate());
    }

private:

    void _Update();

    enMode _Mode;

    std::string _Country;
    std::string _CurrencyCode;
    std::string _CurrencyName;
    double _Rate;
};

// Persistence is delegated to CurrencyRepository below. The header is included
// after the class so the include graph has no cycles: CurrencyRepository.h only
// consumes the fully-defined Currency type.
#include "Repositories/CurrencyRepository.h"

inline std::vector <Currency> Currency::GetAllUSDRates()
{
    return Bank::Data::CurrencyRepository::LoadAll().Records;
}

inline Currency Currency::FindByCode(std::string CurrencyCode)
{
    return Bank::Data::CurrencyRepository::FindByCode(CurrencyCode);
}

inline Currency Currency::FindByCountry(std::string Country)
{
    return Bank::Data::CurrencyRepository::FindByCountry(Country);
}

inline bool Currency::IsCurrencyExist(std::string CurrencyCode)
{
    return Bank::Data::CurrencyRepository::Exists(CurrencyCode);
}

inline std::vector <Currency> Currency::GetCurrenciesList()
{
    return Bank::Data::CurrencyRepository::LoadAll().Records;
}

inline void Currency::_Update()
{
    std::vector <Currency> currencies = Bank::Data::CurrencyRepository::LoadAll().Records;

    for (Currency& C : currencies)
    {
        if (C.CurrencyCode() == CurrencyCode())
        {
            C = *this;
            break;
        }
    }

    Bank::Data::CurrencyRepository::SaveAll(currencies);
}