from PyQt6.QtWidgets import QApplication, QWidget, QPushButton, QVBoxLayout
import json
import sys
from PyQt6.QtWidgets import QApplication, QMainWindow
from PyQt6.QtCharts import QChart, QChartView, QPieSeries


# read from JSON

with open("disk.json") as f:
    raw=json.load(f)
    data = raw[" Disk Info "]  

new_data=[] #data[:3]
for item in data:
    if int(item["size"]) > 0 and item["Directory "].startswith(("/boot")):
        new_data.append(item)
    
app = QApplication(sys.argv)

series=QPieSeries()

total = sum((int(item["size"]) - int(item["avail"])) for item in new_data)
for item in new_data:
    value = int(item["size"]) - int(item["avail"]) 
    pct = (value / total) * 100
    gb = value / (1024**3)
    label = f"{item['Mount']}\n{gb:.2f} GB ({pct:.1f}%)"
    slice_obj = series.append(label, value)
    slice_obj=series.setLabelsVisible(True)
    
chart = QChart()
chart.addSeries(series)
chart.setTitle("From Disk")

#setting gb vals

#slice_obj.setLabel(f"{gb:.1f} GB\n{pct:.1f}%")


view = QChartView(chart)
view.setMinimumSize(400, 300)

window = QMainWindow()
window.setCentralWidget(view)
window.show()

sys.exit(app.exec())

