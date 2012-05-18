# Generate data for each test(local, pos, punctuation..) and evaluate each of them
# Usage: ./eval1.sh datasetname(pizza|hotel|contact|all)
echo "***** Generating dataset and evaluating for $1 *****"
#./eval2.sh $1 Local
#./eval2.sh $1 Geo
#./eval2.sh $1 Pos
#./eval2.sh $1 LocalGeo
#./eval2.sh $1 LocalPos
#./eval2.sh $1 GeoPos
#./eval2.sh $1 LocalGeoPos
#./eval2.sh $1 LocalGeoPosPuncLayout
./eval2.sh $1 LocalGeoPosPuncLayoutFips
#./eval2.sh $1 LocalGeoPosPuncLayoutNoDic
#./eval2.sh $1 LocalGeoPosPuncLayoutRe
#./eval2.sh $1 LocalGeoPosPuncLayoutGeocoder
#./eval2.sh $1 LocalGeoPosPuncLayoutReGeocoder
