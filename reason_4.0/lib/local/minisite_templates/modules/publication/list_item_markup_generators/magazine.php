<?php
/**
 *  @package reason
 *  @subpackage minisite_modules
 */
 
 /**
  * Include the base class
  */
reason_include_once( 'minisite_templates/modules/publication/markup_generator.php' );

/**
*  A basic post listing page that displays the posts' categories after the publication date and images at custom sizes.
*
*/

class MagazinePublicationListItemMarkupGenerator extends PublicationMarkupGenerator
{
	//variables needed to be passed from the publication module
	var $variables_needed = array( 	'use_dates_in_list', 
									'date_format', 
									'item',
									'item_comment_count', 
									'link_to_full_item', 
									'permalink',
									'teaser_image',
									'item_categories',
									'publication',
									'current_issue',
									'current_filters',
								  	'commenting_status',
									);
	
	var $show_section_name = false;

	function PublicationListItemMarkupGenerator ()
	{
	}
	
	function run ()
	{	
		$this->markup_string .= $this->get_teaser_image_markup();
		$this->markup_string .= $this->get_content_block_markup();
	}

	// FULL SIZE TEASER IMG MARKUP
	function get_teaser_image_markup() // {{{
	{
		$markup_string = '';
		$image = current($this->passed_vars['teaser_image']);
		$link_to_full_item = isset($this->passed_vars['link_to_full_item']) ? $this->passed_vars['link_to_full_item'] : '';

		if (!empty($image))
		{

			$markup_string .= '<div class="image-block">';
			$markup_string .= '<figure class="primaryImage">';
			if(isset($link_to_full_item) &&  !empty($link_to_full_item))
				$markup_string .=  '<a href="' .$link_to_full_item. '">';
				
			// if we're showing filtered items...
			if(!empty($this->passed_vars['current_filters']))
			{
				$markup_string .= $this->get_category_teaser_image_markup();
			}
			// else (on the home page)
			else {
				$markup_string .= '<img src="'.luther_get_image_url(WEB_PHOTOSTOCK.reason_get_image_filename( $image->id() )).'" alt="'.str_replace('"', "'", $image->get_value( 'description' )).'"/>';
			}	

			if(isset($link_to_full_item) &&  !empty($link_to_full_item))
				$markup_string .=  '</a>';
			
			$markup_string .= '</figure>';
			$markup_string .= $this->get_item_category_markup();
			$markup_string .= '</div>';
		} 
		else {
			$markup_string .= '<div class="category-block">';
				$markup_string .= $this->get_item_category_markup();
			$markup_string .= '</div>';
		}
		
		return $markup_string;
	}

	// CUSTOM TEASER IMG SIZE MARKUP for CATEGORY PAGES
	function get_category_teaser_image_markup()
	{
		$markup_string = '';
		$image = $this->passed_vars['teaser_image'];
		if (!file_exists($_SERVER['DOCUMENT_ROOT'] . WEB_PHOTOSTOCK.reason_get_image_filename( reset($image)->id(), 'tn' )))
		{
			// not cropped if image is pulled from www to staging or localhost
			$markup_string .= '<img src="'.luther_get_image_url(WEB_PHOTOSTOCK.reason_get_image_filename( reset($image)->id(), "tn" )).'" width = "200" alt="'.str_replace('"', "'", reset($image)->get_value( 'description' )).'"/>';
		}		
		else if (!empty($image))
		{
			if(is_array($image))
				$image = reset($image);
			
			$rsi = new reasonSizedImage();
			$rsi->set_id($image->id());
			$rsi->set_width(200);
			$rsi->set_height(150);
			$rsi->set_crop_style('fill');

			ob_start();	
			show_image( $rsi,true,false,false );
			$markup_string .= ob_get_contents();
			ob_end_clean();
		}
		return $markup_string;
	}

	// CUSTOM TEASER IMG SIZE MARKUP
	// function get_teaser_image_markup()
	// {
	// 	$markup_string = '';
	// 	$image = $this->passed_vars['teaser_image'];
	// 	if (!empty($image))
	// 	{
	// 		$markup_string .= '<figure class="teaserImage">';

	// 		if(is_array($image))
	// 			$image = reset($image);
			
	// 		$rsi = new reasonSizedImage();
	// 		$rsi->set_id($image->id());
	// 		$rsi->set_width(400);
	// 		$rsi->set_height(250);
	// 		$rsi->set_crop_style('fill');

	// 		ob_start();	
	// 		show_image( $rsi,true,false,false );
	// 		$markup_string .= ob_get_contents();
	// 		ob_end_clean();
	// 		//$markup_string .= '<img src="/reason/sized_images/540620/b25879bda30d8a9542e03ab9670e730e.gif?cb=1397165736">';
	// 		$markup_string .= '</figure>';
	// 	}
	// 	return $markup_string;
	// }

	// ORIGINAL TEASER IMG MARKUP	
	// function get_teaser_image_markup() // {{{
	// {
	// 	$markup_string = '';
	// 	$image = $this->passed_vars['teaser_image'];
	// 	if (!empty($image))
	// 	{
	// 		$markup_string .= '<div class="teaserImage">';
	// 		ob_start();	
	// 		show_image( reset($image), true,false,false );
	// 		$markup_string .= ob_get_contents();
	// 		ob_end_clean();
	// 		$markup_string .= '</div>';
	// 	} 
	// 	return $markup_string;
	// }

	function get_content_block_markup() {
		$link_to_full_item = isset($this->passed_vars['link_to_full_item']) ? $this->passed_vars['link_to_full_item'] : '';
		
		$markup_string =  '<div class="content-block">';

		// if we're showing filtered items...
		if(!empty($this->passed_vars['current_filters']))
		{
			$markup_string .= $this->get_issue_date_markup();
		}

		if(isset($link_to_full_item) &&  !empty($link_to_full_item))
			$markup_string .=  '<a href="' .$link_to_full_item. '">';
		
		$markup_string .= $this->get_title_markup();
		$markup_string .= $this->get_description_markup();
		
		if(isset($link_to_full_item) &&  !empty($link_to_full_item))
			$markup_string .=  '</a>';
		$markup_string .=  '</div>';
		return $markup_string;
	}
	
	function get_item_category_markup()
	{
		$link_markup = array();
		$markup_string = '<ul class="post-cats">';
		foreach ($this->passed_vars['item_categories'] as $cat)
		{
			$class_name = $cat->get_value('name');
			// //Lower case everything
			$class_name = strtolower($class_name);
			// //Make alphanumeric (removes all other characters)
			$class_name = preg_replace("/[^a-z0-9_\s-]/", "", $class_name);
			// //Clean up multiple dashes or whitespaces
			$class_name = preg_replace("/[\s-]+/", " ", $class_name);
			// //Convert whitespaces and underscore to dash
			$class_name = preg_replace("/[\s_]/", "-", $class_name);

			$link_markup[] = '<li class="post-cat ' . $class_name . '"><a href="' . carl_make_link( array(), '', '', false, false) . $cat->get_value('category_url') . '">' . $cat->get_value('name') . '</a></li>';
		}
		$markup_string .= implode("\n ", $link_markup);
		$markup_string .= '</ul>';
		return $markup_string;
	}

	function get_title_markup()
	{
		$markup_string = '';
		$item = $this->passed_vars['item'];
		$markup_string .=  '<h4 class="title">';
		$markup_string .= $item->get_value('release_title');
		$markup_string .=  '</h4>'."\n";
		return $markup_string;
	}
	
	function get_issue_date_markup()
	{		
		$item = $this->passed_vars['item'];

		$es = new entity_selector();
		$es->add_type(id_of('issue_type'));
		$es->add_right_relationship($item->_id, relationship_id_of('news_to_issue'));
		$result = $es->run_one();
		foreach( $result AS $id => $issue )
		{
			return '<div class="date">'.$issue->get_value('name').'</div>'."\n";
		}
	}
	
	function get_description_markup()
	{
		$item = $this->passed_vars['item'];
		if($item->get_value('description'))
			return '<div class="desc">'.$item->get_value('description').'</div>'."\n";
	}
}
?>