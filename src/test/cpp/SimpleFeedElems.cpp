#include "SimpleFeedElems.h"

namespace Tests
{

	NotimplFeedItem::NotimplFeedItem() : refs(1) {}
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::Xml(FEEDS_XML_INCLUDE_FLAGS includeFlags, BSTR *xml) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Title(BSTR *title) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Link(BSTR *linkUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Guid(BSTR *itemGuid) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Description(BSTR *description) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_PubDate(DATE *pubDate) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Comments(BSTR *comments) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Author(BSTR *author) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Enclosure(IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_IsRead(VARIANT_BOOL *isRead) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::put_IsRead(VARIANT_BOOL isRead) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_LocalId(LONG *itemId) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Parent(IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::Delete(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_DownloadUrl(BSTR *itemUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_LastDownloadTime(DATE *lastDownload) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::get_Modified(DATE *modified) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE NotimplFeedItem::QueryInterface(const IID &, void **) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE NotimplFeedItem::AddRef(void) { return ++refs; }
	ULONG STDMETHODCALLTYPE NotimplFeedItem::Release(void) { unsigned int retval = --refs; if (retval == 0) { delete this; } return retval; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::GetTypeInfoCount(UINT *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::GetTypeInfo(UINT, LCID, ITypeInfo **) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedItem::Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) { return E_NOTIMPL; }
	
	
	
	NotimplFeedFeed::NotimplFeedFeed() : refs(1) {}
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Title(BSTR *title) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_DownloadUrl(BSTR *feedUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Url(BSTR *feedUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::GetItem(LONG itemId, IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Xml(LONG count, FEEDS_XML_SORT_PROPERTY sortProperty, FEEDS_XML_SORT_ORDER sortOrder, FEEDS_XML_FILTER_FLAGS filterFlags, FEEDS_XML_INCLUDE_FLAGS includeFlags, BSTR *xml) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Name(BSTR *name) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Rename(BSTR name) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::put_Url(BSTR feedUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LocalId(BSTR *feedGuid) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Path(BSTR *path) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Move(BSTR newParentPath) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Parent(IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LastWriteTime(DATE *lastWrite) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Delete(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Download(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::AsyncDownload(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::CancelAsyncDownload(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_SyncSetting(FEEDS_SYNC_SETTING *syncSetting) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::put_SyncSetting(FEEDS_SYNC_SETTING syncSetting) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Interval(LONG *minutes) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::put_Interval(LONG minutes) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LastDownloadTime(DATE *lastDownload) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LocalEnclosurePath(BSTR *path) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Items(IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Description(BSTR *description) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Link(BSTR *homePage) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Image(BSTR *imageUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LastBuildDate(DATE *lastBuildDate) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_PubDate(DATE *lastPopulateDate) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Ttl(LONG *ttl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Language(BSTR *language) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_Copyright(BSTR *copyright) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_MaxItemCount(LONG *count) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::put_MaxItemCount(LONG count) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_DownloadEnclosuresAutomatically(VARIANT_BOOL *downloadEnclosuresAutomatically) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::put_DownloadEnclosuresAutomatically(VARIANT_BOOL downloadEnclosuresAutomatically) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_DownloadStatus(FEEDS_DOWNLOAD_STATUS *status) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_LastDownloadError(FEEDS_DOWNLOAD_ERROR *error) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Merge(BSTR feedXml, BSTR feedUrl) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_IsList(VARIANT_BOOL *isList) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::MarkAllItemsRead(void) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::GetWatcher(FEEDS_EVENTS_SCOPE scope, FEEDS_EVENTS_MASK mask, IDispatch **disp) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_UnreadItemCount(LONG *count) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::get_ItemCount(LONG *count) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::QueryInterface(const IID &, void **) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE NotimplFeedFeed::AddRef(void) { return ++refs; }
	ULONG STDMETHODCALLTYPE NotimplFeedFeed::Release(void) { unsigned int retval = --refs; if (retval == 0) { delete this; } return retval; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::GetTypeInfoCount(UINT *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::GetTypeInfo(UINT, LCID, ITypeInfo **) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE NotimplFeedFeed::Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) { return E_NOTIMPL; }



	ChildrenFeedFeed::ChildrenFeedFeed(std::vector<IFeedItem*> items) : items(items) {}
	HRESULT STDMETHODCALLTYPE ChildrenFeedFeed::get_ItemCount(LONG *count) { *count = items.size(); return S_OK; }
	HRESULT STDMETHODCALLTYPE ChildrenFeedFeed::GetItem(LONG itemId, IDispatch **disp) { items[itemId]->AddRef(); *disp = items[itemId]; return S_OK; }
	HRESULT STDMETHODCALLTYPE ChildrenFeedFeed::get_Items(IDispatch **disp) { *disp = new VectorFeedsEnum(items); return S_OK; }



	VectorFeedsEnum::VectorFeedsEnum(std::vector<IFeedItem*> items) : refs(1), items(items) {}
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::get_Count(LONG *count) { *count = items.size(); return S_OK; }
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::Item(LONG itemId, IDispatch **disp) { items[itemId]->AddRef(); *disp = items[itemId]; return S_OK; }

	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::get__NewEnum(IEnumVARIANT **enumVar) { return E_NOTIMPL; }

	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::QueryInterface(const IID &, void **) { return E_NOTIMPL; }
	ULONG STDMETHODCALLTYPE VectorFeedsEnum::AddRef(void) { return ++refs; }
	ULONG STDMETHODCALLTYPE VectorFeedsEnum::Release(void) { unsigned int retval = --refs; if (retval == 0) { delete this; } return retval; }
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::GetTypeInfoCount(UINT *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::GetTypeInfo(UINT, LCID, ITypeInfo **) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::GetIDsOfNames(const IID &, LPOLESTR *, UINT, LCID, DISPID *) { return E_NOTIMPL; }
	HRESULT STDMETHODCALLTYPE VectorFeedsEnum::Invoke(DISPID, const IID &, LCID, WORD, DISPPARAMS *, VARIANT *, EXCEPINFO *, UINT *) { return E_NOTIMPL; }

}
