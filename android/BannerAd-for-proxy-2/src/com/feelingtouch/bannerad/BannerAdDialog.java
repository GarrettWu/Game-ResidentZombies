package com.feelingtouch.bannerad;

import java.util.LinkedList;
import java.util.List;

import android.app.Dialog;
import android.content.Context;
import android.os.Handler;
import android.os.Message;
import android.view.View;
import android.widget.AdapterView;
import android.widget.AdapterView.OnItemClickListener;
import android.widget.Button;
import android.widget.ListView;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;

import com.feelingtouch.bannerad.load.BannerAdTool;
import com.feelingtouch.bannerad.util.IntentUtil;
import com.feelingtouch.bannerad.util.StringUtil;
import com.feelingtouch.rpc.ads.model.GameAdBanner;

public class BannerAdDialog extends Dialog {
	public BannerAdDialog(Context context) {
		super(context, R.style.customized_dialog);
		setContentView(R.layout.banner_dialog);
		_ctx = context;
		_adapter = new BannerAdapter(_ctx);
		// canShow = false;
		// initView();
	}

	// 注释掉的部分是将来合作伙伴需要的时候，直接弹出空的对话框等待数据取回使用
	// public void init(List<GameAdBanner> list) {
	// initData(list);
	// }

	public void initDialog(List<GameAdBanner> list) {
		initView();
		initData(list);
	}

	protected List<GameAdBanner> _list = null;
	protected static List<BannerAdAdapterItem> _bannerList = new LinkedList<BannerAdAdapterItem>();
	protected ListView _listView;
	protected Button _button;
	protected BannerAdapter _adapter = null;
	protected Context _ctx;
	protected RelativeLayout _layout;
	protected RelativeLayout _titleLayout;
	protected ProgressBar _prograss;

	// private boolean canShow = false;

	public BannerAdAdapterItem getBannerAdAdapterItem() {
		if (_bannerList != null & _bannerList.size() > 0) {
			if (_bannerList.get(0).bmp == null) {
				return null;
			}

			return _bannerList.get(0);
		}

		return null;
	}

	public List<BannerAdAdapterItem> getBannerAdAdapterItemList() {
		if (_bannerList != null & _bannerList.size() > 0) {
			return _bannerList;
		}

		return null;
	}

	private Handler _handler = new Handler() {
		public void handleMessage(Message msg) {
			switch (msg.what) {
			case SET_DIALOG_DATA:
				// canShow = true;
				_adapter.setData(_bannerList);
				_listView.setAdapter(_adapter);
				_listView.setVisibility(View.VISIBLE);
				_titleLayout.setVisibility(View.VISIBLE);
				_prograss.setVisibility(View.GONE);
				break;
			default:
				break;
			}
		}
	};

	// 初始化bannerList中的数据，包括取回游戏图标
	public void initData(List<GameAdBanner> list) {
		if (_bannerList.size() == 0) {
			_list = new LinkedList<GameAdBanner>();
			if (list.size() < 2) {
				_list.add(list.get(0));
			} else {
				_list.add(list.get(0));
				_list.add(list.get(1));
			}
			if (_list != null && _list.size() > 0) {
				for (GameAdBanner item : _list) {
					BannerAdAdapterItem ad = new BannerAdAdapterItem();
					ad.banner = item;
					_bannerList.add(ad);
				}
				Thread t = new Thread() {
					public void run() {
						for (BannerAdAdapterItem item : _bannerList) {
							item.bmp = BannerAdTool
									.getIcon(item.banner.iconLink);

							// 取banner的图片
							if (StringUtil.isNotEmpty(item.banner.bannerLink)) {
								item.bannerBmp = BannerAdTool
										.getIcon(item.banner.bannerLink);
							}
						}
						_handler.sendEmptyMessage(SET_DIALOG_DATA);
					}
				};
				t.start();
			}
		} else {
			_handler.sendEmptyMessage(SET_DIALOG_DATA);
		}
	}

	// 初始化背景框，按钮等等
	public void initView() {
		_layout = (RelativeLayout) findViewById(R.id.layout_root);
		_titleLayout = (RelativeLayout) findViewById(R.id.title_layout);
		if (ImageManager.background != null)
			_layout.setBackgroundDrawable(ImageManager.background);
		_listView = (ListView) findViewById(R.id.game_list);
		_prograss = (ProgressBar) findViewById(R.id.ads_loading);
		_listView.setVisibility(View.GONE);
		if (_adapter != null) {
			_listView.setAdapter(_adapter);
		}

		_listView.setOnItemClickListener(new OnItemClickListener() {
			@Override
			public void onItemClick(AdapterView<?> arg0, View arg1,
					int position, long arg3) {
				dismiss();
				IntentUtil.toDownloadPlatform(_ctx,
						_bannerList.get(position).banner.packageName,
						_bannerList.get(position).banner.marketLink,
						_bannerList.get(position).banner.httpLink);
			}
		});

		_button = (Button) findViewById(R.id.ok);
		_button.setOnClickListener(new View.OnClickListener() {
			@Override
			public void onClick(View v) {
				dismiss();
			}
		});
	}

	// 注释掉的部分是将来合作伙伴需要的时候，直接弹出空的对话框等待数据取回，
	// 因为有可能数据还没有从网络中取回，但是对话框已经弹出
	public void show() {
		if (_adapter != null) {
			_listView.setAdapter(_adapter);
		}
		super.show();
	}

	// ExitAdDialog共用一份图片，所以这个释放了那个就不用释放
	public void release() {
		try {
			if (_bannerList != null) {
				for (BannerAdAdapterItem item : _bannerList) {
					if (item != null && item.bmp != null)
						item.bmp.recycle();

					if (item != null && item.bannerBmp != null)
						item.bannerBmp.recycle();
				}
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

	}

	protected static final int SET_DIALOG_DATA = 1;

}
