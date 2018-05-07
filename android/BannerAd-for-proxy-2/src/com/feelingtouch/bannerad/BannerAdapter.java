package com.feelingtouch.bannerad;

import java.util.List;

import android.content.Context;
import android.view.LayoutInflater;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.ViewGroup;
import android.widget.BaseAdapter;
import android.widget.Button;
import android.widget.ImageView;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.feelingtouch.bannerad.util.IntentUtil;

public class BannerAdapter extends BaseAdapter {
	private List<BannerAdAdapterItem> _list;
	private LayoutInflater _inflater;
	private Context _ctx;

	private boolean isExitDialog = false;

	public BannerAdapter(Context ctx) {
		_inflater = LayoutInflater.from(ctx);
		_ctx = ctx;
	}

	public void setData(List<BannerAdAdapterItem> list) {
		_list = list;
	}

	public void setIsExitDialog(boolean b) {
		isExitDialog = b;
	}

	@Override
	public int getCount() {
		if (_list == null) {
			return 0;
		}
		return _list.size();
	}

	@Override
	public Object getItem(int position) {
		return _list.get(position);
	}

	@Override
	public long getItemId(int position) {
		return position;
	}

	@Override
	public View getView(int position, View convertView, ViewGroup parent) {
		if (_list == null) {
			return null;
		}
		View view = null;
		try {
			if (convertView == null) {
				BannerAdAdapterItem ad = _list.get(position);
				final int pos = position;

				if (isExitDialog) {
					view = _inflater.inflate(R.layout.banner_exit_list_item,
							parent, false);
				} else {
					view = _inflater.inflate(R.layout.banner_list_item, parent,
							false);
				}

				RelativeLayout layout = (RelativeLayout) view
						.findViewById(R.id.layout);
				layout.setBackgroundDrawable(ImageManager.item_bg);

				final ImageView image = (ImageView) view
						.findViewById(R.id.game_icon);
				TextView text = (TextView) view.findViewById(R.id.game_desc);
				TextView name = (TextView) view.findViewById(R.id.game_name);
				Button button = (Button) view.findViewById(R.id.download);

				if (ad.bmp != null)
					image.setImageBitmap(ad.bmp);

				if (ImageManager.button_bg != null)
					button.setBackgroundDrawable(ImageManager.button_bg);

				text.setText(ad.banner.desc);
				name.setText(ad.banner.gameName);
				button.setOnClickListener(new OnClickListener() {
					@Override
					public void onClick(View v) {
						download(pos);
					}
				});
			} else {
				view = convertView;
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

		return view;
	}

	private void download(int pos) {
		IntentUtil.toDownloadPlatform(_ctx, _list.get(pos).banner.packageName,
				_list.get(pos).banner.marketLink,
				_list.get(pos).banner.httpLink);
	}

}
