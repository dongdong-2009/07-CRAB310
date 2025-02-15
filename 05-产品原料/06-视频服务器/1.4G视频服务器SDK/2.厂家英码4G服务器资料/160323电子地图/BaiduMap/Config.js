/****************************************************************************
 *	定义全局常量
 *
 ****************************************************************************/


var DEFAULT_ACTIVE_NAVIGATIONCONTROL = 1;		// 是否初始化地图平移缩放控件
var DEFAULT_ACTIVE_SCALECONTROL = 1;			// 是否初始化比例尺控件
var DEFAULT_ACTIVE_OVERVIEWMAPCONTROL = 1;		// 是否初始化缩略图控件
var DEFAULT_ACTIVE_MAPTYPECONTROL = 1;			// 是否初始化地图类型控件
var DEFAULT_ACTIVE_TRAFFICCONTROL = 1;			// 是否初始化交通控件

var DEFAULT_MAP_GETLOCATEFORMIP = 1;			// 地图初始化的时候是否采用IP定位 若为1，则启用IP定位，而手动初始定位不生效
var DEFAULT_MAP_POINTX = 116.404;				// 地图初始中心经度
var	DEFAULT_MAP_POINTY = 39.915;				// 地图初始中心纬度
var DEFAULT_MAP_ZOOM = 13;						// 地图初始缩放大小

var DEFAULT_MARKERICON_WIDTH = 25;				// 自定义标记图标默认宽度
var DEFAULT_MARKERICON_HEIGHT = 25;				// 自定义标记图标默认高度
var DEFAULT_MARKERICON_ANCHORX = 12.5;			// 当标注显示在地图上时，其所指向的地理位置距离图标左上的横坐标偏移量
var DEFAULT_MARKERICON_ANCHORY = 25;			// 当标注显示在地图上时，其所指向的地理位置距离图标左上的纵坐标偏移量
var DEFAULT_MARKERICON_INFOWINDOWANCHORX = 25;	// 信息窗口开启位置相对于图标左上角的横坐标偏移值
var DEFAULT_MARKERICON_INFOWINDOWANCHORY = 0;	// 信息窗口开启位置相对于图标左上角的纵坐标偏移值
var DEFAULT_MARKER_DBCLICK_ZOOM = 13;			// 双击标记时，缩放大小; 若为0则不启用

var DEFAULT_LABEL_LIMITSHOWNZOOM = 13;			// Label标签显示的最小缩放大小
var DEFAULT_LABEL_OFFSETX = 12.5;				// Label标签相对于标注图标左上角的的偏移值
var DEFAULT_LABEL_OFFSETY = 25;					// Label标签相对于标注图标左上角的的偏移值

var DEFAULT_INFOWINDOW_WIDTH = 0;				// InfoWindow默认的宽，若宽为0则自适应
var DEFAULT_INFOWINDOW_HEIGHT = 0;				// InfoWindow默认的高，若高为0则自适应
var DEFAULT_INFOWINDOW_NULLINFO = "无详细信息";

var DEFAULT_POLYLINE_TAIL_STROKECOLOR = "#3399FF";
var DEFAULT_POLYLINE_TAIL_STROKEWEIGHT = 6;
var DEFAULT_POLYLINE_TAIL_STROKEOPACITY = 0.7;

var DEFAULT_ICONLIST = [{						// 默认icon数组
					Name : "online_Northicon",
					Icon : {
						ImageUrl : "Image/caroverlook64North.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_Southicon",
					Icon : {
						ImageUrl : "Image/caroverlook64South.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_Westicon",
					Icon : {
						ImageUrl : "Image/caroverlook64West.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_Easticon",
					Icon : {
						ImageUrl : "Image/caroverlook64East.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_SouthEasticon",
					Icon : {
						ImageUrl : "Image/caroverlook64SouthEast.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_SouthWesticon",
					Icon : {
						ImageUrl : "Image/caroverlook64SouthWest.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_NorthWesticon",
					Icon : {
						ImageUrl : "Image/caroverlook64NorthWest.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{						
					Name : "online_NorthEasticon",
					Icon : {
						ImageUrl : "Image/caroverlook64NorthEast.gif",
						Width : 30,
						Height : 30,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 15,
						AnchorY : 15,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "offline_icon",
					Icon : {
						ImageUrl : "Image/MarkerPink24.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "online_icon",
					Icon : {
						ImageUrl : "Image/MarkerAzure24.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "unlocated_icon",
					Icon : {
						ImageUrl : "Image/MarkerChartreuse24.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "temphum_icon",
					Icon : {
						ImageUrl : "Image/tempvalue.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "mrd_icon",
					Icon : {
						ImageUrl : "Image/mrd24.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
						}
					},{
					Name : "tower_icon",
					Icon : {
						ImageUrl : "Image/towersensor.gif",
						Width : 25,
						Height : 25,
						ImageOffsetX : 0,
						ImageOffsetY : 0,
						AnchorX : 12.5,
						AnchorY : 25,
						InfoWindowAnchorX : 10,
						InfoWindowAnchorY : 0
					}
					}];