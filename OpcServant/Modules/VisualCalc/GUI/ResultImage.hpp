#ifndef RESULTIMAGE_HPP
#define RESULTIMAGE_HPP

#include <VisualCalc/NodeEngine/NE_EvaluationEnv.hpp>
#include <VisualCalc/NodeUIEngine/NUIE_DrawingImage.hpp>

class ResultImage
{
public:
	ResultImage ();
	virtual ~ResultImage ();

	bool	IsModified () const;
	void	Draw (NUIE::DrawingContext& context) const;

	void	AddItem (const NUIE::DrawingItemConstPtr& item);
	void	RemoveItem (const NUIE::DrawingItemConstPtr& item);
	void	Invalidate ();
	void	Validate ();
	void	Clear ();

private:
	NUIE::DrawingImage	drawingImage;
	bool				modified;
};

class ResultImageEvaluationData : public NE::EvaluationData
{
public:
	ResultImageEvaluationData (const std::shared_ptr<ResultImage>& resultImage);
	virtual ~ResultImageEvaluationData ();

	std::shared_ptr<ResultImage>&	GetResultImage ();
	void							AddItem (const NUIE::DrawingItemConstPtr& item);
	void							RemoveItem (const NUIE::DrawingItemConstPtr& item);
    unsigned id() const { return _id;}
private:
	std::shared_ptr<ResultImage>	resultImage;
    unsigned _id = 0; // link to the RTObject
};


#endif
