#ifndef PRODUCTS
#define PRODUCTS

struct Product {
    bool ndvi;
    bool ndwi;
    bool svi;
    bool distance;
    bool kt;
    bool density;
    bool risk;
    Product(): ndvi(false)
        , ndwi(false)
        , svi(false)
        , distance(false)
        , kt(false)
        , density(false)
        , risk(false)
    {

    }
};

#endif // PRODUCTS

