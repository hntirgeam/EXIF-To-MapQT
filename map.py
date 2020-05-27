import folium
from folium.plugins import HeatMap

m = folium.Map(location=[47.963, 24.055], tiles="OpenStreetMap", zoom_start = 4)
hm = folium.Map(location=[47.963, 24.055], tiles="OpenStreetMap", zoom_start = 4)

markers = []

def create_markers():
    try:
        readfile = open("gps_coordinates.txt", "r")
        readfile_split = readfile.read().splitlines()
        i = 0
        while i < len(readfile_split):
            latitude, longitude, altitude = readfile_split[i].split(' ')
            folium.Marker([latitude, longitude]).add_to(m)
            temp = [latitude, longitude]
            markers.append(temp)
            i += 1

        HeatMap(markers, radius = 25).add_to(hm)
        hm.save('heatMap.html')
        m.save('map.html')
    finally:
        readfile.close()


create_markers()
print("Done, opening browser")
